#include"../h/syscall_cpp.hpp"

void* operator new(size_t size) {
    return mem_alloc(size);
}

void* operator new[](size_t size) {
    return mem_alloc(size);
}

void operator delete(void* ptr){
    mem_free(ptr);
}   

void operator delete[](void* ptr){
    mem_free(ptr);
}

Thread::Thread(void (*body)(void*), void* arg): myHandle(nullptr), body(body), arg(arg) {}

Thread::Thread(): myHandle(nullptr), body(threadWrapper), arg(this) {}

Thread::~Thread() {}

void Thread::start() {
    thread_create(&myHandle, body, arg);
}

void Thread::threadWrapper(void* thread) {
    ((Thread*)thread)->run();
}

void Thread::dispatch() {
    thread_dispatch();
}





