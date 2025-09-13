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

int Thread::sleep(time_t time) {
    return time_sleep(time);
}

void Thread::dispatch() {
    thread_dispatch();
}

Semaphore::Semaphore(unsigned init){
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore(){
    sem_close(&myHandle);
}

int Semaphore::wait(){
    return sem_wait(myHandle);
}

int Semaphore::signal(){
    return sem_signal(myHandle);
}


void PeriodicThread::periodicActivation() {}

PeriodicThread::PeriodicThread(time_t period) : Thread(), period(period) {}

void PeriodicThread::terminate() {
    period = 0;
}

void PeriodicThread::run() {
    while (period) {
        periodicActivation();

        sleep(period);
    }
}






