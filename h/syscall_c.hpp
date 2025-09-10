#ifndef _syscall_c_hpp
#define _syscall_c_hpp

#include "../lib/hw.h"



enum syscall_codes {
    MEM_ALLOC = 0x01,
    MEM_FREE = 0x02,
    MEM_GET_FREE_SPACE = 0x03,
    MEM_GET_LARGEST_FREE_BLOCK = 0x04,
    THREAD_CREATE = 0x11,
    THREAD_EXIT = 0x12,
    THREAD_DISPATCH = 0x13,
    SEM_OPEN = 0x21,
    SEM_CLOSE = 0x22,
    SEM_WAIT = 0x23,
    SEM_SIGNAL = 0x24,
    SEM_TIMEDWAIT = 0x25,
    SEM_TRYWAIT = 0x26,
    TIME_SLEEP = 0x31,
    GETC = 0x41,
    PUTC = 0x42
};


void* mem_alloc(size_t size);

int mem_free(void* ptr);

size_t mem_get_free_space();

size_t mem_get_largest_free_block();

class _thread;

typedef _thread* thread_t;

int thread_create(thread_t* handle, void (*start_routine)(void*), void* arg);

int thread_exit();

void thread_dispatch();

typedef unsigned long time_t;

int time_sleep(time_t time);



#endif // _syscall_c_hpp