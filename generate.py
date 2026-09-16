#!/usr/bin/env python3
"""Generate a file of pseudo-random integers, one per line, for benchmarking volsort.

Usage: generate.py COUNT OUTPUT [SEED]

Values are drawn from [1, 2^31 - 1) so that every line parses cleanly with
std::stoi, and so that duplicate values are rare (a duplicate-heavy input
would push the linked-list quicksort toward its O(n^2) worst case).
"""

import random
import sys

BATCH = 1 << 20
HI = (1 << 31) - 2


def main():
    if len(sys.argv) not in (3, 4):
        sys.exit("usage: generate.py COUNT OUTPUT [SEED]")

    count = int(sys.argv[1])
    path = sys.argv[2]
    seed = int(sys.argv[3]) if len(sys.argv) == 4 else 42

    rand = random.Random(seed)
    randrange = rand.randrange

    with open(path, "w") as handle:
        remaining = count
        while remaining > 0:
            n = min(BATCH, remaining)
            handle.write("\n".join(str(randrange(1, HI)) for _ in range(n)))
            handle.write("\n")
            remaining -= n


if __name__ == "__main__":
    main()
