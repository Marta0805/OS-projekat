// MemoryAllocator.cpp
#include "../h/MemoryAllocator.hpp"

MemoryAllocator::MemoryBlock* MemoryAllocator::freeListHead = nullptr;

size_t MemoryAllocator::freeMemory = 0;

size_t MemoryAllocator::largestFreeBlock = 0;

MemoryAllocator::MemoryAllocator() {
    size_t totalBytes  = (size_t)HEAP_END_ADDR - (size_t)HEAP_START_ADDR;
    size_t totalBlocks = totalBytes / MEM_BLOCK_SIZE;

    freeListHead = (MemoryBlock*)HEAP_START_ADDR;
    if (totalBlocks >= HEADER_BLOCKS) {
        freeListHead->size = totalBlocks - HEADER_BLOCKS;
    } else {
        freeListHead->size = 0; 
    }
    freeListHead->next = nullptr;

    freeMemory = freeListHead->size;
    largestFreeBlock = freeListHead->size;
}

MemoryAllocator* MemoryAllocator::getInstance() {
    static MemoryAllocator instance;
    return &instance;
}

void* MemoryAllocator::mem_alloc(size_t size) {
    if (size == 0 || size > freeMemory) return nullptr;

    MemoryBlock* prev = nullptr;
    MemoryBlock* curr = freeListHead;

    while (curr) {
        if (curr->size >= size) {
            if (curr->size >= size + HEADER_BLOCKS + 1) {
                char* base = (char*)curr;

                MemoryBlock* newBlock = (MemoryBlock*)(
                    base + (HEADER_BLOCKS + size) * (size_t)MEM_BLOCK_SIZE
                );

                newBlock->size = curr->size - size - HEADER_BLOCKS;
                newBlock->next = curr->next;

                if (prev) prev->next = newBlock;
                else      freeListHead = newBlock;

                curr->size = size;
            } else {
                if (prev) prev->next = curr->next;
                else      freeListHead = curr->next;

                size = curr->size; 
            }

            freeMemory -= size;

            largestFreeBlock = 0;
            for (MemoryBlock* b = freeListHead; b; b = b->next)
                if (b->size > largestFreeBlock) largestFreeBlock = b->size;

            return (void*)((char*)curr + HEADER_BLOCKS * (size_t)MEM_BLOCK_SIZE);
        }
        prev = curr;
        curr = curr->next;
    }
    return nullptr; 
}

size_t MemoryAllocator::mem_free(void* ptr) {
    if (!ptr) return 0;

    MemoryBlock* block = (MemoryBlock*)((char*)ptr - HEADER_BLOCKS * (size_t)MEM_BLOCK_SIZE);
    size_t payloadBlocks = block->size;

    if ((size_t)block < (size_t)HEAP_START_ADDR ||
        (size_t)block >= (size_t)HEAP_END_ADDR)
        return 0;

    MemoryBlock* prev = nullptr;
    MemoryBlock* curr = freeListHead;

    while (curr && (uint64)curr < (uint64)block) {
        prev = curr;
        curr = curr->next;
    }

    bool adjLeft  = prev &&
        ((char*)prev + (HEADER_BLOCKS + prev->size) * (size_t)MEM_BLOCK_SIZE == (char*)block);
    bool adjRight = curr &&
        ((char*)block + (HEADER_BLOCKS + block->size) * (size_t)MEM_BLOCK_SIZE == (char*)curr);

    if (adjLeft) {
        prev->size += HEADER_BLOCKS + block->size;
        block = prev;
    } else {
        block->next = curr;
        if (prev) prev->next = block;
        else      freeListHead = block;
    }

    if (adjRight) {
        block->size += HEADER_BLOCKS + curr->size;
        block->next  = curr->next;
    }

    freeMemory += payloadBlocks;
    if (block->size > largestFreeBlock) largestFreeBlock = block->size;

    return payloadBlocks;
}

size_t MemoryAllocator::mem_get_free_space() {
    return freeMemory;
}

size_t MemoryAllocator::mem_get_largest_free_block() {
    return largestFreeBlock;
}
