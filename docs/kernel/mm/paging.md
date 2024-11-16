### **Paging Overview in CeanOS**

Paging is a memory management scheme that eliminates the need for contiguous allocation of physical memory, thus helping to avoid fragmentation. This system allows the kernel to map virtual addresses to physical addresses and manage memory more efficiently. The key elements in CeanOS’s paging system are:

- **Page Directory**: A structure that holds pointers to page tables, allowing the system to manage large virtual memory spaces.
- **Page Tables**: A structure used to map virtual addresses to physical addresses.
- **Physical Memory Bitmap**: A bitmap that tracks allocated and free physical memory pages.

---

### **Global Variables**

- `pageFrameMin`: The minimum address of the physical memory frames.
- `pageFrameMax`: The maximum address of the physical memory frames.
- `totalAlloc`: The total number of allocated physical memory frames.
- `mem_num_vpages`: The total number of virtual pages.
- `physicalMemoryBitmap[]`: A dynamically allocated bit array to keep track of used/free page frames.
- `pageDirs[]`: An array of page directory entries.
- `pageDirUsed[]`: An array that tracks which page directories are in use.

---

### **Functions**

#### `pmm_init(uint32_t memLow, uint32_t memHigh)`
Initializes the paging system. It sets the range for physical memory frames and clears the bitmap that tracks allocated frames.

- **Parameters**:
  - `memLow`: The starting address of the available physical memory.
  - `memHigh`: The ending address of the available physical memory.

- **How it works**:
  - It calculates `pageFrameMin` and `pageFrameMax` by dividing `memLow` and `memHigh` by the page size (0x1000).
  - It clears the physical memory bitmap.

#### `memGetCurrentPageDir()`
Returns the address of the current page directory, which is stored in the `CR3` register.

- **Returns**: The address of the current page directory.

#### `memChangePageDir(uint32_t* pd)`
Changes the current page directory by modifying the `CR3` register.

- **Parameters**:
  - `pd`: A pointer to the new page directory.

#### `syncPageDirs()`
Synchronizes the page directories by clearing certain flags in the entries of page directories that are in use.

#### `memMapPage(uint32_t virtualAddr, uint32_t physAddr, uint32_t flags)`
Maps a virtual address to a physical address and applies specific flags.

- **Parameters**:
  - `virtualAddr`: The virtual address to map.
  - `physAddr`: The physical address to map to.
  - `flags`: Flags like `PAGE_FLAG_PRESENT`, `PAGE_FLAG_WRITE`, etc.
  
- **How it works**:
  - It checks if the page directory for the given virtual address is already present. If not, it allocates a new page table.
  - It then maps the page and invalidates the TLB to ensure the new mapping takes effect.

#### `pmmAllocPageFrame()`
Allocates a free physical page frame from the available memory range.

- **Returns**: The physical address of the allocated page frame.

- **How it works**:
  - It scans through the `physicalMemoryBitmap` to find the first free page frame and marks it as used.
  - It increments the total allocation count and returns the physical address of the allocated page.

#### `initMemory(uint32_t memHigh, uint32_t physicalAllocStart)`
Initializes the memory system, including setting up page directories and invalidating entries in the Translation Lookaside Buffer (TLB).

- **Parameters**:
  - `memHigh`: The high address of available memory.
  - `physicalAllocStart`: The starting address for physical memory allocation.

- **How it works**:
  - Sets up the initial page directory.
  - Calls `pmm_init()` to initialize the physical memory allocator.
  - Initializes the page directory structures and sets the first and last entries of the initial page directory.

#### `invalidate(uint32_t vaddr)`
Invalidates the TLB for a given virtual address, forcing the system to reload the page table entry for that address.

- **Parameters**:
  - `vaddr`: The virtual address to invalidate.

---

### **Page Directory Structure**

The page directory is an array of entries, where each entry points to a page table that maps virtual pages to physical pages. In this system:

- `NUM_PAGES_DIRS = 256`: Defines the number of page directories.
- Each page directory contains 1024 entries, and each entry points to a page table.

---

### **Flags for Page Entries**

- `PAGE_FLAG_PRESENT`: Indicates that the page is present in memory.
- `PAGE_FLAG_WRITE`: Marks the page as writable.
- `PAGE_FLAG_OWNER`: Used to indicate ownership of the page.
- Additional flags can be applied as needed during memory mapping.

---

### **Usage Example**

To map a virtual address to a physical address, you can call `memMapPage()` with the desired virtual address, physical address, and flags. This will ensure that the page is mapped correctly, and any necessary page tables or directories are created automatically.

---

### **Considerations**

- **Memory Allocation**: The `pmmAllocPageFrame()` function ensures that physical pages are allocated efficiently by maintaining a bitmap of available frames.
- **TLB Invalidations**: After modifying page mappings, the `invalidate()` function ensures that the processor’s translation lookaside buffer (TLB) is updated to reflect changes.
