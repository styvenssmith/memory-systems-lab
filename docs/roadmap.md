

The progression follows how allocators are normally assembled: raw OS memory → block metadata → reuse → splitting/coalescing → size classes → small-object slabs → concurrency → production diagnostics. [dmitrysoshnikov](https://dmitrysoshnikov.com/compilers/writing-a-memory-allocator/)

## Final destination

```text
Linux custom allocator in C
├── malloc/free/calloc/realloc-style API
├── mmap-backed memory acquisition
├── alignment-aware allocation
├── small-object slabs / size classes
├── medium free lists with split + coalesce
├── direct mmap for large allocations
├── per-thread arenas + remote free path
├── debug checks, stats, heap validator
├── tests, fuzzing, benchmarks
└── optional C++ std::pmr adapter / LD_PRELOAD library
```

## Roadmap

| Phase | Learn | Build | Done when |
|---|---|---|---|
| 0. Setup | C, compiler warnings, sanitizers, Git, `gdb` | `memory-labs` repo, CMake/Make target, test target | You can compile with warnings-as-errors and ASan/UBSan |
| 1. Memory foundations | Pointers, arrays, `sizeof`, `alignof`, padding, stack vs dynamic storage | Layout inspector + pointer exercises | You can explain alignment and struct padding |
| 2. C++ lifetime foundations | Construction, destruction, raw storage vs object lifetime, RAII | C++ lifetime tracker + placement-construction lab | You can explain storage ≠ object |
| 3. Linux virtual memory | Pages, virtual memory, `mmap`, `munmap`, `mprotect`, `/proc` | `mmap` explorer | You can map, touch, inspect, and unmap pages |
| 4. One-mapping allocator | Prefix headers, overflow checks, API contracts | `my_malloc` / `my_free`: one `mmap` per allocation | You can draw `[header][payload]` and explain free |
| 5. Fixed-buffer arena | Bump pointer, alignment rounding, bulk lifetime | Arena with `alloc`, `reset`, exhaustion handling | Allocation is \(O(1)\); reset invalidates all allocations |
| 6. Stack allocator | Markers, LIFO lifetime | `mark()` / `rewind()` stack arena | You can reclaim allocations in reverse order |
| 7. Fixed-size pool | Intrusive free list, block reuse | `ObjectPool` for one fixed size | Allocate/free is \(O(1)\); blocks are reused |
| 8. Bitmap pool | Bitsets, slot ownership, validation | Bitmap-backed pool | You can find/mark/free slots correctly |
| 9. Single-heap free list | Block headers, first-fit, splitting | One `mmap` heap segment with free-list allocation | Different allocation sizes work and split correctly |
| 10. Boundary tags | Physical neighbors, `prev_size`, block merging | Coalescing allocator | Adjacent freed blocks merge correctly |
| 11. Segregated bins | Size classes, fragmentation tradeoffs | Binned free-list allocator | Freed blocks go into correct bins; split/merge updates bins |
| 12. Large allocations | Threshold routing, direct mapping | Large allocations use direct `mmap`/`munmap` | Large path is correct and independently tested |
| 13. Small-object slabs | Slabs, pages, bitmaps/free stacks, internal fragmentation | Slab allocator for small sizes | Fast small allocations and slab reuse work |
| 14. Full single-threaded allocator | Tier dispatch, `calloc`, `realloc`, stats | Slab + medium bins + large mmap tiers | Full API works under randomized tests |
| 15. Debug allocator | Red zones, canaries, poison patterns, leak tracking | Debug mode + `validate_heap()` | Corruption and invalid frees are caught where possible |
| 16. Threading | TLS, locks, arena ownership, cross-thread free | Per-thread arenas + remote-free queue | Stress test works with many threads |
| 17. Measurement | Benchmark design, latency, RSS, page faults, `perf` | Benchmark suite vs system allocator | You can explain measured tradeoffs |
| 18. C++ integration | PMR, allocator-aware containers | `std::pmr::memory_resource` adapter | `std::pmr::vector` works with your allocator |
| 19. Optional hard mode | Dynamic loader, reentrancy, ABI behavior | `LD_PRELOAD` shared library | You document support and limitations honestly |

## Project grouping

Think of the roadmap as five increasingly serious projects.

### Project 1: Memory labs

**Phases 0–4**

Goal: Understand raw memory and make a correct, intentionally slow allocator.

```text
layout inspector
→ lifetime tracker
→ mmap explorer
→ one-mmap-per-allocation allocator
```

Deliverable:

```c
void* my_malloc(size_t size);
void  my_free(void* ptr);
```

Implementation model:

```text
[ header with mapping size ][ caller payload ]
```

No free lists. No threads. No `realloc`. No "optimization."

## Project 2: Specialized allocators

**Phases 5–8**

Goal: Learn how allocator strategy follows an allocation lifetime pattern.

```text
arena
→ stack arena
→ fixed-size object pool
→ bitmap pool
```

Deliverables:

```c
void* arena_alloc(struct arena*, size_t bytes, size_t alignment);
void  arena_reset(struct arena*);

void* pool_alloc(struct pool*);
void  pool_free(struct pool*, void*);
```

At the end, you should know when to use:

- Arena: everything dies together
- Stack allocator: lifetimes are last-in, first-out
- Pool: same-size objects are frequently reused

A monotonic resource uses the arena model: allocations advance through storage and individual deallocation is not the normal operation; bulk release handles reclamation. [indico.gsi](https://indico.gsi.de/event/24256/contributions/97033/attachments/54162/83525/20260304_Cpp_UG_Meeting_Allocators.pdf)

## Project 3: General single-threaded allocator

**Phases 9–12**

Goal: Build the hard core of a general allocator.

```text
one heap segment
→ first-fit scan
→ split a large free block
→ boundary tags
→ coalesce neighbors
→ segregated size bins
→ direct mmap for large allocations
```

This is the first time you build something meaningfully comparable to a simplified `malloc`.

Core invariant:

> After `free`, no two physically adjacent free blocks remain uncoalesced.

Free-list links can live inside the payload area while the block is free, reducing allocation overhead. Boundary tags give fast access to neighboring blocks for coalescing. [dmitrysoshnikov](https://dmitrysoshnikov.com/compilers/writing-a-memory-allocator/)

## Project 4: Full single-threaded allocator

**Phases 13–15**

Goal: Add practical performance, API completeness, and diagnosability.

```text
small-object slabs
→ tiered dispatch
→ calloc
→ realloc
→ stats
→ canaries
→ heap validation
→ fuzz / randomized stress tests
```

Final architecture:

```text
request
  │
  ├── small  → size-class slab/pool
  ├── medium → segregated free lists + split/coalesce
  └── large  → direct mmap
```

Deliverable API:

```c
void* my_malloc(size_t size);
void  my_free(void* ptr);
void* my_calloc(size_t count, size_t size);
void* my_realloc(void* ptr, size_t new_size);

struct alloc_stats my_allocator_stats(void);
bool my_allocator_validate(void);
```

## Project 5: Concurrent production-style allocator

**Phases 16–19**

Goal: Make the allocator capable of real multi-threaded workloads and demonstrate engineering maturity.

```text
thread-local arena
→ owner tracking
→ remote-free queue
→ global arena fallback
→ benchmarks
→ PMR wrapper
→ optional LD_PRELOAD
```

Deliverable architecture:

```text
Thread A
  │
  └── arena A: small slabs + medium bins

Thread B
  │
  └── arena B: small slabs + medium bins

Free on a different thread:
  Thread B frees memory owned by A
      → enqueue in A's remote-free queue
      → A reclaims it safely
```

PMR is the C++ integration endpoint: `std::pmr::polymorphic_allocator` holds a runtime-selected `memory_resource`, allowing allocator-aware containers to use your resource. [indico.gsi](https://indico.gsi.de/event/24256/contributions/97033/attachments/54162/83525/20260304_Cpp_UG_Meeting_Allocators.pdf)

## Required knowledge gates

Do not advance because you "finished some code." Advance only when you can explain the gate.

| Before this phase | You must already be able to answer |
|---|---|
| Phase 4 | Why does `free` need metadata, and why is it stored before the payload? |
| Phase 5 | What is alignment rounding, and why can naïve pointer bumps misalign an object? |
| Phase 7 | Why does a free-list pointer belong in a free block's unused payload? |
| Phase 9 | What is the difference between raw OS memory and allocator-managed blocks? |
| Phase 10 | How do you find previous and next physical blocks, and what are the four coalescing cases? |
| Phase 11 | What is internal vs external fragmentation? |
| Phase 13 | Why are pools/slabs better than general free lists for repeated small allocation sizes? |
| Phase 14 | How should `calloc` handle multiplication overflow? What must `realloc` preserve? |
| Phase 16 | Why does per-thread allocation not automatically make cross-thread free safe? |
| Phase 17 | What exact workload and metric prove the allocator is better or worse? |
| Phase 18 | What is the difference between a C allocator API and a C++ PMR memory resource? |

## Final deliverable checklist

Your final large project is finished when it has all of these:

- Linux-only scope clearly documented.
- C API for `malloc`, `free`, `calloc`, and `realloc` semantics.
- 16-byte or `alignof(max_align_t)` baseline alignment guarantee.
- Small, medium, and large allocation routes.
- Correct splitting and coalescing for medium blocks.
- Reusable small-object slab/pool allocation.
- Direct `mmap`/`munmap` large-object path.
- Overflow-safe size arithmetic.
- Tested `NULL`, zero-size, failure, alignment, boundary-size, and `realloc` paths.
- Heap validator and debug mode.
- Randomized allocation trace/fuzz tests.
- Thread-local arenas and safe cross-thread freeing.
- Statistics: allocations, frees, live requested bytes, mapped bytes, peak use, bin/slab state.
- Benchmarks against the system allocator under defined workloads.
- Clear documentation of tradeoffs and limitations.
- Optional `std::pmr::memory_resource` integration.
- Optional `LD_PRELOAD` interposition only after the custom API is complete.

## Suggested pacing

This is a **6–9 month deep project** if you are learning the concepts properly, building tests, debugging, reading systems material, and documenting results.

| Month | Main target |
|---|---|
| 1 | Phases 0–4: foundations + one-mmap allocator |
| 2 | Phases 5–8: arena, stack, pool, bitmap |
| 3 | Phases 9–10: free list, splitting, coalescing |
| 4 | Phases 11–12: bins and large-allocation path |
| 5 | Phases 13–14: slabs, full API, single-threaded tiered allocator |
| 6 | Phase 15: diagnostics, validation, fuzzing, tests |
| 7 | Phase 16: threads and remote frees |
| 8 | Phase 17: profiling, benchmark suite, tuning |
| 9 | Phases 18–19: PMR integration, optional preload, final documentation |

## One rule

At every phase, produce:

1. A working executable or library.
2. Automated tests.
3. A diagram of the memory layout.
4. A list of invariants.
5. A short README: design, limitations, and what the next phase adds.

That repository history—from a 30-line `mmap` allocator through free lists, slabs, concurrency, tests, and benchmarks—will tell a much stronger story than dropping a single giant allocator codebase at the end.
