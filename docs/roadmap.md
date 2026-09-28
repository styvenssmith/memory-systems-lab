# Memory Systems Roadmap

## End goal

Build a Linux-focused, production-style custom memory allocator in C:

- `my_malloc`
- `my_free`
- `my_calloc`
- `my_realloc`
- mmap-backed memory acquisition
- small-object slabs
- segregated free lists
- splitting and coalescing
- direct mmap path for large allocations
- diagnostics and heap validation
- thread-local arenas and safe remote frees
- benchmarks and an optional C++ PMR adapter

## Phases

- [ ] 00. Toolchain and repository setup
- [ ] 01. Object layout and alignment
- [ ] 02. C++ object lifetime and raw storage
- [ ] 03. Linux virtual-memory exploration
- [ ] 04. One-mmap-per-allocation allocator
- [ ] 05. Fixed-buffer arena
- [ ] 06. Stack allocator
- [ ] 07. Fixed-size object pool
- [ ] 08. Bitmap pool
- [ ] 09. Single free-list allocator
- [ ] 10. Boundary tags and coalescing
- [ ] 11. Segregated free lists
- [ ] 12. Direct mmap for large allocations
- [ ] 13. Slab allocator for small objects
- [ ] 14. Full single-threaded allocator API
- [ ] 15. Debug mode, validation, and fuzzing
- [ ] 16. Thread-local arenas and remote frees
- [ ] 17. Benchmarking and profiling
- [ ] 18. C++ PMR integration
- [ ] 19. Optional LD_PRELOAD interposer
