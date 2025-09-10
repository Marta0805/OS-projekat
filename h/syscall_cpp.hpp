#ifndef _syscall_cpp_hpp
#define _syscall_cpp_hpp

#include "syscall_c.hpp"

void* operator new(size_t size);

void operator delete(void* ptr);

class Thread{
    public:
        Thread(void (*body)(void*), void* arg);

        virtual ~Thread();

        void start();

        //static int sleep(time_t);

        static void dispatch();

        static void threadWrapper(void*);


    protected:
        Thread();

        virtual void run() {}
    
    private:

        thread_t myHandle;

        void (*body)(void*);

        void* arg;
};


#endif // _syscall_cpp_hpp