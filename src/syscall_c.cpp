#include"../h/syscall_c.hpp"

#include"../h/MemoryAllocator.hpp"

#include"../h/RiscV.hpp"


void* mem_alloc(size_t size) {
    if(size == 0) return nullptr;

    size_t blockSize = ((size + MEM_BLOCK_SIZE - 1 + MemoryAllocator::HEADER_BLOCKS)) / MEM_BLOCK_SIZE;

    __asm__ volatile("mv a1, %[blockSize]" : : [blockSize] "r"(blockSize));
    __asm__ volatile("mv a0, %0" : : "r"(MEM_ALLOC));

    __asm__ volatile("ecall");

    void* volatile ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));

    return ret;
}

int mem_free(void* ptr) {
    if(ptr == nullptr) return -1;  

    __asm__ volatile("mv a1, %[ptr]" : : [ptr] "r"(ptr));
    __asm__ volatile("mv a0, %0" : : "r"(MEM_FREE));

    __asm__ volatile("ecall");

    size_t volatile ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));

    return ret;
}

size_t mem_get_free_space() {
    __asm__ volatile("mv a0, %0" : : "r"(MEM_GET_FREE_SPACE));

    __asm__ volatile("ecall");

    size_t volatile ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));

    return ret;
}

size_t mem_get_largest_free_block() {
    __asm__ volatile("mv a0, %0" : : "r"(MEM_GET_LARGEST_FREE_BLOCK));

    __asm__ volatile("ecall");

    size_t volatile ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));

    return ret;
}

int thread_create(thread_t* handle, void (*body)(void*), void* arg) {
    void* stack = mem_alloc(DEFAULT_STACK_SIZE);
    if(stack == nullptr) return -1;

    __asm__ volatile("mv a4, %[stack]" : : [stack] "r"((uint64)stack));
    
    __asm__ volatile("mv a3, %[arg]" : : [arg] "r"(arg));

    __asm__ volatile("mv a2, %[body]" : : [body] "r" (body));

    __asm__ volatile("mv a1, %[handle]" : : [handle] "r"(handle));

    __asm__ volatile("mv a0, %0" : : "r"(THREAD_CREATE));

    __asm__ volatile("ecall");

    int volatile ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return ret;
}

int thread_exit() {
    __asm__ volatile("mv a0, %0" : : "r"(THREAD_EXIT));

    __asm__ volatile("ecall");

    int volatile ret;

    __asm__ volatile("mv %0, a0" : "=r"(ret));

    return ret;
}

void thread_dispatch() {
    __asm__ volatile("mv a0, %0" : : "r"(THREAD_DISPATCH));

    __asm__ volatile("ecall");
}