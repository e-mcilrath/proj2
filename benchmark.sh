#!/bin/bash
#
# Benchmark every volsort sorting mode across a range of input sizes and emit a
# Markdown table of elapsed wall-clock time and peak resident memory.
#
#   ./benchmark.sh
#
# Environment overrides:
#   DATA_DIR    where the generated input files live   (default /tmp/$USER-volsort)
#   OUT         Markdown table destination             (default benchmark.md)
#   SIZES       space-separated input sizes
#   MODES       space-separated sorting modes
#   SORT_FLAGS  extra flags passed to volsort          (default -n)
#   TIMEOUT     per-run wall-clock cap in seconds      (default 1800)
#   TIME_BIN    GNU time binary                        (default /usr/bin/time)
#   KEEP_DATA   set to 0 to delete the inputs when done

set -u

DATA_DIR=${DATA_DIR:-/tmp/$USER-volsort}
OUT=${OUT:-benchmark.md}
SIZES=${SIZES:-"100000 500000 1000000 10000000 25000000"}
MODES=${MODES:-"stl qsort merge quick"}
SORT_FLAGS=${SORT_FLAGS:--n}
TIMEOUT=${TIMEOUT:-1800}
KEEP_DATA=${KEEP_DATA:-1}
TIME_BIN=${TIME_BIN:-/usr/bin/time}

# The linked-list quicksort recurses once per partition level, so give it room.
ulimit -s unlimited 2>/dev/null

# Preconditions ---------------------------------------------------------------

for tool in "$TIME_BIN" timeout python3; do
    if ! command -v "$tool" >/dev/null 2>&1; then
        echo "error: required tool '$tool' not found" >&2
        exit 1
    fi
done

TIME_VERSION=$("$TIME_BIN" --version 2>&1 | head -1)

# GNU time <= 1.7 runs ru_maxrss (already KiB on Linux) through a pages->KiB
# conversion a second time, inflating %M by pagesize/1024 = 4x on 4K-page Linux.
# Fixed upstream in 1.8, but Debian/Ubuntu backported the fix into packages that
# still report "1.7", so the version string alone cannot be trusted. Calibrate
# against a process with a known footprint instead: allocate 256 MiB and see
# what %M claims.
RSS_DIVISOR=1
calib=$(mktemp)
if "$TIME_BIN" -f '%M' -o "$calib" python3 -c 'a=bytearray(256*1024*1024)' 2>/dev/null; then
    reported=$(tail -1 "$calib")
    case "$reported" in
        ''|*[!0-9]*) echo "warning: could not calibrate %M (got '$reported'); assuming KiB" >&2 ;;
        *) if [ "$reported" -gt 655360 ]; then   # >2.5x the 262144 KiB actually used
               RSS_DIVISOR=4
               echo "note: $TIME_VERSION overreports %M by 4x; correcting."
           fi ;;
    esac
else
    echo "warning: %M calibration run failed; assuming KiB" >&2
fi
rm -f "$calib"

make --no-print-directory volsort || exit 1
mkdir -p "$DATA_DIR" || exit 1

# Inputs ----------------------------------------------------------------------

for size in $SIZES; do
    file="$DATA_DIR/$size.txt"
    if [ -s "$file" ] && [ "$(wc -l < "$file")" -eq "$size" ]; then
        echo "reusing $file"
    else
        echo "generating $file ($size integers)"
        ./generate.py "$size" "$file" || exit 1
    fi
done

# Benchmark -------------------------------------------------------------------

stats=$(mktemp)
trap 'rm -f "$stats"' EXIT

{
    echo "| Mode  | Size     | Elapsed Time | Memory   |"
    echo "|-------|----------|--------------|----------|"
} > "$OUT"

for mode in $MODES; do
    for size in $SIZES; do
        file="$DATA_DIR/$size.txt"
        printf 'running %-5s %-9s ... ' "$mode" "$size"

        "$TIME_BIN" -f '%e %M' -o "$stats" \
            timeout "$TIMEOUT" ./volsort -m "$mode" $SORT_FLAGS < "$file" > /dev/null
        status=$?

        elapsed=0; maxrss=0
        read -r elapsed maxrss < <(tail -1 "$stats") || true

        if [ $status -eq 124 ]; then
            secs="> ${TIMEOUT} s (timeout)"
            mem="-"
        elif [ $status -ne 0 ]; then
            secs="failed (exit $status)"
            mem="-"
        else
            secs=$(printf '%.2f s' "$elapsed")
            mem=$(awk -v kb="$maxrss" -v d="$RSS_DIVISOR" 'BEGIN { printf "%.1f MB", kb / d / 1024 }')
        fi

        printf '%s, %s\n' "$secs" "$mem"
        printf '| %-5s | %8s | %12s | %8s |\n' "$mode" "$size" "$secs" "$mem" >> "$OUT"
    done
done

# Report ----------------------------------------------------------------------

{
    echo ""
    cores=$(command -v nproc >/dev/null && nproc || echo "?")
    cpu=$(grep -m1 "model name" /proc/cpuinfo 2>/dev/null | cut -d: -f2- | sed "s/^ *//")
    flags=$(grep -m1 "^CXXFLAGS" Makefile | tr -s "\t " " ")
    echo "Host: $(uname -sr), ${cores} cores, CPU ${cpu:-unknown}"
    echo ""
    echo "Built with \`${flags}\`, timed with \`${TIME_VERSION}\`, sorted numerically (\`${SORT_FLAGS}\`)."
} >> "$OUT"

# Splice the table into README.md between the benchmark markers, if present.
README=${README:-README.md}
if [ -f "$README" ] && grep -q 'BENCHMARK:BEGIN' "$README"; then
    python3 - "$README" "$OUT" <<'PYEOF'
import sys
readme, table = sys.argv[1], sys.argv[2]
body = open(readme).read()
begin, end = '<!-- BENCHMARK:BEGIN -->', '<!-- BENCHMARK:END -->'
i, j = body.index(begin) + len(begin), body.index(end)
open(readme, 'w').write(body[:i] + '\n' + open(table).read().strip() + '\n' + body[j:])
PYEOF
    echo "spliced results into $README"
fi

echo ""
echo "wrote $OUT"
cat "$OUT"

if [ "$KEEP_DATA" = "0" ]; then
    rm -rf "$DATA_DIR"
fi
