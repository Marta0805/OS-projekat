#ifndef _MEMORY_ALLOCATOR_HPP
#define _MEMORY_ALLOCATOR_HPP

#include "../lib/hw.h"

class MemoryAllocator {
public:
    static MemoryAllocator* getInstance();

    static void* mem_alloc(size_t size);

    static size_t mem_free(void* ptr);
 
    static size_t mem_get_free_space();

    static size_t mem_get_largest_free_block();

    struct MemoryBlock {
        MemoryBlock* next;
        size_t size;
    };

    static constexpr size_t HEADER_BLOCKS = (sizeof(MemoryBlock) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;

private:

    static MemoryBlock* freeListHead;

    static size_t freeMemory;

    static size_t largestFreeBlock;

    MemoryAllocator();

    MemoryAllocator(const MemoryAllocator&) = delete;
    MemoryAllocator& operator=(const MemoryAllocator&) = delete;    

};


#endif // _MEMORY_ALLOCATOR_HPP