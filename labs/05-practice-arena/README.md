# Practice Arena Allocator

A learning implementation of a fixed-buffer arena allocator in C.

## Goal

Practice:

- raw memory buffers
- pointer arithmetic
- alignment
- bump-pointer allocation
- capacity checks
- bulk reclamation with `arena_reset()`

## Status

Work in progress. This is a learning implementation, not a production allocator.

## Files

- `Makefile`: builds and runs the exercise
- `arena.c`: arena implementation
- `arena.h`: public API
- `test_arena.c`: tests and experiments

## Invariant

Successful allocations are contiguous within a backing buffer and remain valid until the arena is reset or destroyed.
