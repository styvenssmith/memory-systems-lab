Experiment 1
1. My system page size is 16384 bytes
2. the program queries the OS at runtime using 
sysconf(_SC_PAGESIZE)

Experiment 2
1. mmap creates a region in the process's virtual address space. nullptr lets the OS choose the virtual address. The length requests four pages of memory. PROT_READ |PROT_WRITE allows reading and writing. MAP_PRIVATE makes modifications private using copy on write. MAP_ANONYMOUS means the memory is not backed by a file. Since there is no file, the file descriptor is -1 and the offset is 0.


Experiment 3
1. We touch one byte per page table, to understand that mmap() does not immediately allocate physical RAM because it only creates a virtual memory mapping. The OS uses demand paging, so physical pages are allocated when the progam first accesses each page. mmap() does not construct C++ objects; it only provides raw storage
