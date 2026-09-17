#!/usr/bin/env python3
"""generate.py - Eric McIlrath, Abe Rashdan (emcilrat, arashdan)

makes a file of random numbers for the benchmark

usage: generate.py COUNT OUTPUT [SEED]

everything stays under 2^31 so stoi is happy, and with that big of a range we
hardly ever get duplicates, which would slow our quick sort way down.
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
