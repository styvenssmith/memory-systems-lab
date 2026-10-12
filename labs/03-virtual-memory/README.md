Experiment 1
    1. My system page size is 16384 bytes
    2. the program queries the OS at runtime using 
sysconf(_SC_PAGESIZE)

Experiment 2
    1. mmap creates a region in the process's virtual address space. nullptr lets the OS choose the virtual address. The length requests four pages of memory. PROT_READ |PROT_WRITE allows reading and writing. MAP_PRIVATE makes modifications private using copy on write. MAP_ANONYMOUS means the memory is not backed by a file. Since there is no file, the file descriptor is -1 and the offset is 0.


Experiment 3
    1. We touch one byte per page table, to understand that mmap() does not immediately allocate physical RAM because it only creates a virtual memory mapping. The OS uses demand paging, so physical pages are allocated when the progam first accesses each page. mmap() does not construct C++ objects; it only provides raw storage


Experiment 4
Question:
    - How can I see the memory region that mmap created
answer:
    - /proc/self/maps


Experiment 5
Question:
    1. We want to know what problem does munmap() solve
Answer:
    2. tells the kernel I'm done with this mapping

    3. munmap() needs the original address returned by mmap() because we need the actual start of the original data we allocated, if the memory address is less or more than the start we will accidentally remove memory we did not mean to access, we need the original length for the same reason to know the precise memory we want to remove
    4. munmap(memory, length)
        - find the mapping that begins at memory
        - removes that mapping from your process's virtual address space
        - invalidates the page table entries for that region
        - frees physical pages if no longer needed
        - returns 0 on success


Experiment 6 placement construction
    - mmap() gives you the warehouse
    - construct_at() put the object on the shelf, the allocator decides where shelves exist





































































