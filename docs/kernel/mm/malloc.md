### Malloc Overview:
This code explains CeanOS `kmalloc`. It uses a free-list-based allocator and relies on page frame management for allocating physical memory pages. It also provides functionality to resize the heap and free allocated memory.

---

### Key Functions:

1. **`init_heap(void *heap_start, size_t heap_size)`**
   - **Description:** Initializes the heap by setting up the first memory block in the free list. It takes the starting address (`heap_start`) and the size of the heap (`heap_size`).
   - **Parameters:**
     - `heap_start`: Starting address of the heap.
     - `heap_size`: Total size of the heap.
   - **Functionality:**
     - Sets the first block in the free list to span the entire heap size and marks it as free.

2. **`kmallocInit(uint32_t initialHeapSize)`**
   - **Description:** Initializes the kernel memory allocator. This function sets the heap's starting address and its initial size, then increases the heap size as needed.
   - **Parameters:**
     - `initialHeapSize`: The initial heap size to be allocated.
   - **Functionality:**
     - Initializes internal variables such as the starting address (`heapStart`) and heap size (`heapSize`).
     - Uses `changeHeapSize` to allocate necessary space.
   
3. **`changeHeapSize(int newSize)`**
   - **Description:** Adjusts the heap size by allocating or deallocating page frames as necessary.
   - **Parameters:**
     - `newSize`: The new heap size.
   - **Functionality:**
     - Compares the current heap size with the new size, calculating the number of pages that need to be added or removed.
     - Allocates or deallocates page frames accordingly.

4. **`kmalloc(size_t size)`**
   - **Description:** Allocates memory of the specified size from the free list. If no suitable free block is found, it returns `NULL`.
   - **Parameters:**
     - `size`: The number of bytes to allocate.
   - **Returns:** A pointer to the allocated memory.
   - **Functionality:**
     - Searches the free list for a block large enough to accommodate the requested size.
     - If the block is larger than required, it splits the block, keeping the remaining space as another free block.
     - Maps the required pages of physical memory for the allocated virtual memory space.
   
5. **`kfree(void* ptr)`**
   - **Description:** Frees the previously allocated memory pointed to by `ptr`. It merges adjacent free blocks in the free list.
   - **Parameters:**
     - `ptr`: The pointer to the memory to be freed.
   - **Functionality:**
     - Marks the memory block as free.
     - Unmaps the associated physical pages.
     - Merges adjacent free blocks to reduce fragmentation in the free list.

6. **`memUnmapPage(uint32_t virtualAddr)`**
   - **Description:** Unmaps a virtual address from the page tables and invalidates the translation in the Translation Lookaside Buffer (TLB).
   - **Parameters:**
     - `virtualAddr`: The virtual address to be unmapped.
   - **Functionality:**
     - Finds the corresponding entry in the page table for the virtual address and sets it to `0`.
     - Invalidates the TLB entry for the address.

---

### Data Structures:

- **`memory_block` struct:**
   - Represents a block of memory in the free list.
   - **Fields:**
     - `free`: A boolean indicating whether the block is free (`true`) or allocated (`false`).
     - `size`: The size of the memory block.
     - `next`: Pointer to the next `memory_block` in the free list.

- **Global Variables:**
   - **`heapStart`**: The starting address of the heap.
   - **`heapSize`**: The total size of the heap.
   - **`threshold`**: A threshold for triggering heap expansion (though unused in the current code).
   - **`kmallocInitalized`**: A flag indicating if the allocator has been initialized.
   - **`free_list`**: A pointer to the first block in the free list.
  
---

### Error Handling:

- **`kmalloc`:** Returns `NULL` if there is no sufficient space in the free list.
- **`kfree`:** Does nothing if the pointer is `NULL`.

---

### Important Notes:

- The allocator is designed to work in a kernel environment where page-level memory management is handled separately. Physical pages are allocated and mapped as needed.
- The heap is dynamically managed, allowing for resizing and allocating/freeing memory blocks of varying sizes.

---

### Example Usage:

```c
// initialize heap and kmalloc
uint32_t mod1 = *(uint32_t*)(boot->mods_addr + 4);
uint32_t physicalAllocStart = (mod1 + 0xFFF) & ~0xFFF;
initMemory(boot->mem_upper * 1024, physicalAllocStart);
kmallocInit(0x1000);
init_heap((int*)KERNEL_MALLOC, 0x600000);

// works the same as in standard C
void *ptr = kmalloc(256); 

kfree(ptr); 
```

---

This provides a basic kernel memory management system with functionality to initialize the heap, allocate and free memory blocks, and adjust the heap size dynamically using physical page management.