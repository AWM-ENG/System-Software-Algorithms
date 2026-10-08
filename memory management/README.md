# Custom C Dynamic Memory Allocator

A lightweight C memory allocator that manages a static 1KB memory pool using a custom heap layout, doubly-linked free list, memory alignment, and automatic block merging.

---

## Key Features

1. **Explicit Doubly-Linked Free List**  
   Tracks memory blocks using a `Header` struct containing block size, status (`is_free`), and pointers to the previous (`prevheader`) and next (`postheader`) blocks.

2. **8-Byte Memory Alignment**  
   Rounds up all requested byte sizes to the nearest multiple of 8 using bitwise masking `((size + 7) & ~7)`. Uses `_Alignas(8)` on the memory pool array to prevent invalid pointer access and hardware performance penalties.

3. **Dynamic Block Splitting**  
   Uses a **First-Fit** allocation strategy. When a free block is larger than needed, it splits the block into two: one allocated block and one remaining free block.

4. **Bidirectional Coalescing (Block Merging)**  
   When memory is freed, it automatically merges adjacent free blocks (left, right, or both) to prevent memory fragmentation.

5. **Safety Checks**  
   Includes out-of-bounds address validation, double-free protection, and safe `NULL` returns on Out-of-Memory (OOM) errors.

---

## API Functions

### `void init_memory(void)`
Initializes the memory pool by creating one large free block that spans the entire static array.

### `void *add_memory(size_t size)`
Allocates an 8-byte aligned block of memory.  
- **Parameters:** `size` - Requested bytes to allocate.  
- **Returns:** A pointer to the usable memory payload, or `NULL` if allocation fails or invalid size is passed.

### `void remove_memory(void *start)`
Frees an allocated block and merges it with neighboring free blocks if available.  
- **Parameters:** `start` - Pointer to the memory payload returned by `add_memory()`.

### `void print_memory(void)`
Prints the complete state of the memory pool to the terminal, showing block addresses, sizes, and status (`FREE` or `ALLOCATED`).

---

## How to Use

```c
#include <stdio.h>

int main(void) {
    // 1. Initialize the memory pool
    init_memory();

    // 2. Allocate memory blocks
    void *ptr1 = add_memory(12); // Allocated as 16 bytes (aligned)
    void *ptr2 = add_memory(30); // Allocated as 32 bytes (aligned)

    // Print current state
    print_memory();

    // 3. Free memory and trigger block merging
    remove_memory(ptr1);
    remove_memory(ptr2);

    // Print state after freeing
    print_memory();

    return 0;
}



