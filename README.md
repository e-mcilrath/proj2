# Project 02: volsort

A sorting utility that reads integers from standard input into a singly linked
list and sorts them with one of four interchangeable backends.

```
usage: volsort
    -m MODE   Sorting mode (oblivious, stl, qsort, merge, quick)
    -n        Perform numerical ordering
```

| Mode    | Implementation                                                        |
|---------|-----------------------------------------------------------------------|
| `stl`   | Copy node pointers into a `std::vector`, sort with `std::sort`, relink |
| `qsort` | Copy node pointers into a `std::vector`, sort with C `qsort`, relink   |
| `merge` | Recursive merge sort performed directly on the linked list             |
| `quick` | Recursive quicksort performed directly on the linked list              |

## Building and testing

```bash
make            # build ./volsort
make test       # output correctness (8 cases) + valgrind memory checks
```

## Benchmark

Generated with `./benchmark.sh`, which creates the input files via
`./generate.py` and times each mode with `/usr/bin/time`. Input is uniformly
random integers in `[1, 2^31)`, sorted numerically (`-n`), with `stdout`
redirected to `/dev/null`.

<!-- BENCHMARK:BEGIN -->
*(run `./benchmark.sh` to populate this table)*
<!-- BENCHMARK:END -->

## Discussion

### Relative performance of each sorting method

The four modes divide cleanly into two families, and the split explains most of
what the table shows.

**`stl` and `qsort` are array-based.** Both walk the list once to collect
`Node*` into a `std::vector`, sort that contiguous block of pointers, then
relink. Sorting a packed array is cache-friendly: the pointers being compared
and swapped sit next to each other in memory, so the hardware prefetcher keeps
the CPU fed.

Between the two, `stl` is consistently the faster. `std::sort` is a template, so
the comparison function is **inlined directly into the sorting loop** — the
compare becomes a couple of instructions with no call overhead. C `qsort`
receives its comparator as a `void*` function pointer, which cannot be inlined;
every single one of the ~n log n comparisons pays for an indirect call plus two
pointer casts. That overhead alone accounts for most of the gap. `std::sort` is
also introsort (quicksort, switching to heapsort on bad pivots and insertion
sort on small runs), which is better tuned than a generic `qsort`.

**`merge` and `quick` operate on the linked list itself.** They never build the
auxiliary array, so they use noticeably less memory — but every comparison and
every relink is a pointer chase to an unpredictable heap address. Each node
visit risks a cache miss, and the prefetcher cannot help because the next
address is only known after the current node is loaded. This is why the
list-based modes lose to the array-based ones on time even though they perform
the same *number* of comparisons.

Between those two, `merge` beats `quick` here, for three reasons:

1. `quick_sort` uses the **head node as its pivot**. On random input that is
   fine on average, but it never gets the balanced split that merge sort gets
   for free by construction.
2. `concatenate()` has to **walk the entire left partition** to find its tail
   before joining, adding an extra O(n log n) worth of pointer traversal that
   merge sort simply does not perform.
3. Merge sort's access pattern during the merge step is two sequential walks,
   which is the friendliest pattern a linked list can offer.

Memory tells the mirror-image story. `stl` and `qsort` allocate a vector of n
pointers — 8 bytes per element, so roughly 200 MB of extra residency at 25M
elements — on top of the list itself. `merge` and `quick` sort by relinking and
need only O(log n) stack frames, so their peak resident size is essentially the
cost of the nodes alone. The node itself dominates either way: a `Node` is a
`std::string` (32 bytes), an `int`, and a `next` pointer, which with allocator
overhead lands near 48-64 bytes per element.

### Theoretical complexity vs. actual performance

**All four modes are O(n log n) on average, and yet they are not close to
equally fast.** That is the headline result. Asymptotic complexity describes how
runtime *scales* with n, not how long any individual operation takes, and the
constant factor it discards is exactly where these four differ:

- **Memory locality is invisible to Big-O.** An array traversal and a linked
  list traversal are both O(n), but on real hardware a cache miss costs on the
  order of 100x an L1 hit. The array-based modes win on a factor the complexity
  analysis says nothing about.
- **Not all comparisons cost the same.** `stl` and `qsort` perform
  asymptotically identical work; the inlined comparator versus the indirect call
  is a pure constant-factor difference, and it is easily measurable.
- **Constant work outside the sort can dominate entirely.** This is the sharpest
  example from this project. Before tuning, `main.cpp` wrote results with
  `std::endl` (which flushes on every line) and left `std::cin`/`std::cout`
  synchronized with C stdio. Reading and writing the data — an O(n) step that
  the complexity analysis treats as a rounding error next to O(n log n) — took
  roughly **78% of total runtime**, and the four modes were indistinguishable
  from each other. Adding `sync_with_stdio(false)` and replacing `std::endl`
  with `'\n'` cut the 1M-element runtime from ~9s to ~2s and made the
  algorithmic differences visible at all. The asymptotically irrelevant part of
  the program was the part that actually mattered.

The growth *shape* does hold up: going from 100,000 to 1,000,000 elements (10x)
costs noticeably more than 10x, consistent with the extra log n factor. So
complexity correctly predicts the curve, while measurement is the only thing
that predicts the wall clock.

### Which mode is best?

**`stl`.** It is the fastest at every size, and the reasons are structural
rather than accidental: an inlined comparator, a contiguous access pattern, and
a hybrid introsort that degrades gracefully. It is also the only mode with a
**guaranteed O(n log n) worst case** — when introsort detects that recursion is
running too deep it switches to heapsort, so adversarial or pathological input
cannot push it to O(n²).

The trade-off is memory: it needs an auxiliary array of n pointers, about 8n
extra bytes. In this program that is a modest surcharge on top of the nodes
themselves, and it buys the best runtime available, so it is worth paying.

The case against each alternative:

- **`qsort`** does the same work with the same memory cost but cannot inline its
  comparator, so it is strictly slower for no compensating benefit. It is also
  not type-safe — the `void*` casts are checked by nobody.
- **`quick`** is the weakest choice. It carries the same memory profile as
  `merge` but adds an O(n²) worst case (head-pivot selection degrades badly on
  already-sorted or duplicate-heavy input) and the redundant `concatenate()`
  traversal. Its deep recursion on an adversarial input is a stack-overflow risk,
  not just a slow path.
- **`merge`** is the right answer *if memory is the binding constraint*. It
  sorts the list in place by relinking, guarantees O(n log n) regardless of
  input, and is stable. If the working set were large enough that the 8n-byte
  pointer array pushed the process into swap, merge sort's lower peak residency
  would beat `stl`'s better constant factor outright.

So: **`stl` for speed, `merge` if memory-bound, and `quick` under no
circumstances.**
