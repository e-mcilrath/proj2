# Project 02: volsort

Eric McIlrath, Abe Rashdan (emcilrat, arashdan)

Repo: https://github.com/e-mcilrath/proj2

Reads lines off stdin into a linked list and sorts them one of four ways. All
of the sorts move the nodes around by changing the next pointers, we never copy
values back into the nodes.

```
usage: volsort
    -m MODE   Sorting mode (oblivious, stl, qsort, merge, quick)
    -n        Perform numerical ordering
```

```bash
make            # build ./volsort
make test       # the diffs and the valgrind checks
./benchmark.sh  # makes the input files and fills in the table below
```

## Benchmark

<!-- BENCHMARK:BEGIN -->
| Mode  | Size     | Elapsed Time | Memory   |
|-------|----------|--------------|----------|
| stl   |   100000 |       0.16 s |   6.7 MB |
| stl   |   500000 |       1.01 s |  28.2 MB |
| stl   |  1000000 |       2.19 s |  55.0 MB |
| stl   | 10000000 |      24.94 s | 537.7 MB |
| stl   | 25000000 |      66.07 s | 1341.8 MB |
| qsort |   100000 |       0.21 s |   6.8 MB |
| qsort |   500000 |       1.03 s |  28.2 MB |
| qsort |  1000000 |       2.27 s |  55.0 MB |
| qsort | 10000000 |      25.32 s | 537.7 MB |
| qsort | 25000000 |      65.37 s | 1341.8 MB |
| merge |   100000 |       0.21 s |   6.0 MB |
| merge |   500000 |       1.13 s |  24.4 MB |
| merge |  1000000 |       2.40 s |  47.4 MB |
| merge | 10000000 |      27.86 s | 461.6 MB |
| merge | 25000000 |      74.92 s | 1151.5 MB |
| quick |   100000 |       0.20 s |   6.0 MB |
| quick |   500000 |       1.37 s |  24.4 MB |
| quick |  1000000 |       3.35 s |  47.4 MB |
| quick | 10000000 |      52.75 s | 461.6 MB |
| quick | 25000000 |     149.71 s | 1151.5 MB |

Host: Darwin 25.3.0 arm64, sorted numerically (`-n`), output sent to /dev/null.
<!-- BENCHMARK:END -->

## 1. Relative performance of each sorting method

stl and qsort were the fastest and were pretty much tied. Both of them dump the
node pointers into a vector first, and sorting a vector is faster since
everything is right next to each other in memory instead of having to follow
pointers all over the heap. Merge sort was a little behind them. Quick sort was
way slower than everything else, about twice as long at 10 and 25 million. We
think that is because we use the head as the pivot so the splits are never even,
and concatenate walks all the way down the left list every time it puts the two
sides back together.

Memory went the other way. stl and qsort have to hold that extra vector of
pointers on top of the list, so they were around 1340 MB at 25 million. Merge
and quick just relink nodes we already have, so they were around 1150 MB.

## 2. What the results say about theoretical complexity vs actual performance

These results reveal that complexity analysis could be misleading. Although
mergesort is guaranteed n log n even in its worst case, the q sort was more
efficient on our data even though it has a worst case of n^2. This shows that
analyzing only worst case situations can ignore lists that are somewhat
organized or other intermediate cases.

## 3. Which mode is the best?

If our concern is memory then merge sort is probably our best option. If our
main concern is based on average speed then qsort is the obvious answer. It was
much faster than both of the sorts we wrote ourselves and about tied with stl,
but the caveat to that is if it does handle a worst case scenario it will have a
time complexity of n^2.

## Group member contributions

Both members contributed on list.cpp, stl_sort, merge_sort. Eric focused on
quick sort and Abe completed q sort. All other aspects were done together
including the right up.
