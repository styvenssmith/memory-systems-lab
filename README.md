# Memory Systems Lab

A bottom-up systems-programming repository for learning memory deeply:

1. C and C++ object layout, alignment, and lifetime
2. Linux virtual memory with `mmap` and `munmap`
3. Allocator primitives: arenas, stack allocators, pools, and free lists
4. A custom Linux memory allocator in C
5. Testing, diagnostics, benchmarking, concurrency, and C++ PMR integration

## Final project

The final deliverable is a Linux-focused, production-style custom allocator with:

- a malloc-style C API
- mmap-backed memory acquisition
- small-object slabs
- segregated free lists for medium allocations
- coalescing and fragmentation control
- direct mappings for large allocations
- debug validation and statistics
- thread-local arenas and remote-free handling
- an optional `std::pmr::memory_resource` adapter

## Repository structure

```text
docs/       Design notes, roadmap, invariants, and decisions
labs/       Small isolated learning projects
allocator/  Final allocator implementation
scripts/    Development, test, and analysis helpers
```

## Build philosophy

Every milestone must include:

- a memory-layout diagram
- documented invariants
- automated tests
- sanitizer runs
- a short README describing constraints and tradeoffs

## Current milestone

**Phase 00 — Toolchain and repository setup**

Next: build `labs/01-layout`, an object-layout and alignment inspector.
