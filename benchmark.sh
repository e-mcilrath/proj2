#!/bin/bash
# benchmark.sh
# Eric McIlrath, Abe Rashdan (emcilrat, arashdan)
# makes the input files and times every mode, dumps a markdown table

set -u

DATA_DIR=${DATA_DIR:-/tmp/$USER-volsort}
OUT=${OUT:-benchmark.md}
SIZES=${SIZES:-"100000 500000 1000000 10000000 25000000"}
MODES=${MODES:-"stl qsort merge quick"}
SORT_FLAGS=${SORT_FLAGS:--n}

# /usr/bin/time is GNU time on the lab machines but BSD time on a mac, and BSD
# time doesnt know -f, so fall back to gtime if thats what we have
TIME_BIN=${TIME_BIN:-}
if [ -z "$TIME_BIN" ]; then
    if /usr/bin/time -f '%e' true >/dev/null 2>&1; then
        TIME_BIN=/usr/bin/time
    else
        TIME_BIN=$(command -v gtime) || { echo "error: need GNU time"; exit 1; }
    fi
fi

# quick sort recurses a lot so give it whatever stack we can get
ulimit -s unlimited 2>/dev/null || ulimit -s 65520 2>/dev/null

make --no-print-directory volsort || exit 1
mkdir -p "$DATA_DIR" || exit 1

# make the input files (skip any we already made)
for size in $SIZES; do
    file="$DATA_DIR/$size.txt"
    if [ -s "$file" ] && [ "$(wc -l < "$file")" -eq "$size" ]; then
        echo "reusing $file"
    else
        echo "generating $file ($size integers)"
        ./generate.py "$size" "$file" || exit 1
    fi
done

stats=$(mktemp)
trap 'rm -f "$stats"' EXIT

{
    echo "| Mode  | Size     | Elapsed Time | Memory   |"
    echo "|-------|----------|--------------|----------|"
} > "$OUT"

for mode in $MODES; do
    for size in $SIZES; do
        printf 'running %-5s %-9s ... ' "$mode" "$size"

        # %e is seconds, %M is peak memory in KB
        "$TIME_BIN" -f '%e %M' -o "$stats" \
            ./volsort -m "$mode" $SORT_FLAGS < "$DATA_DIR/$size.txt" > /dev/null
        status=$?

        read -r elapsed maxrss < "$stats"

        if [ $status -ne 0 ]; then
            secs="failed (exit $status)"
            mem="-"
        else
            secs="$elapsed s"
            mem=$(awk -v kb="$maxrss" 'BEGIN { printf "%.1f MB", kb / 1024 }')
        fi

        printf '%s, %s\n' "$secs" "$mem"
        printf '| %-5s | %8s | %12s | %8s |\n' "$mode" "$size" "$secs" "$mem" >> "$OUT"
    done
done

{
    echo ""
    echo "Host: $(uname -srm), sorted numerically (\`$SORT_FLAGS\`), output sent to /dev/null."
} >> "$OUT"

# drop the table into the README between the markers
if [ -f README.md ] && grep -q 'BENCHMARK:BEGIN' README.md; then
    python3 - README.md "$OUT" <<'PYEOF'
import sys
readme, table = sys.argv[1], sys.argv[2]
body = open(readme).read()
begin, end = '<!-- BENCHMARK:BEGIN -->', '<!-- BENCHMARK:END -->'
i, j = body.index(begin) + len(begin), body.index(end)
open(readme, 'w').write(body[:i] + '\n' + open(table).read().strip() + '\n' + body[j:])
PYEOF
    echo "spliced results into README.md"
fi

echo ""
cat "$OUT"
