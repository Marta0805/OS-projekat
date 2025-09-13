#include"../h/TCB.hpp"
#include"../h/RiscV.hpp"
#include"../h/syscall_c.hpp"
#include"../lib/hw.h"
#include"../h/Scheduler.hpp"
#include"../h/MemoryAllocator.hpp"

#include "../lib/console.h"

TCB* TCB::running = nullptr;


TCB::~TCB() {
    if(stack){
        MemoryAllocator::getInstance()->mem_free(stack);
    };
}   


int TCB::createHandleTCB(TCB** handle, Body body, void* arg, void* stack) {
    MemoryAllocator* memAlloc = MemoryAllocator::getInstance();
    size_t blockSize = ((sizeof(TCB) + MEM_BLOCK_SIZE - 1 + MemoryAllocator::HEADER_BLOCKS)) / MEM_BLOCK_SIZE;
    *handle = (TCB*)memAlloc->mem_alloc(blockSize);
    (*handle)->body = body;
    if(body){
        (*handle)->stack = (uint64*)stack;
        (*handle)->context.ra = (uint64)(&threadWrapper);
        (*handle)->context.sp = (uint64)&(*handle)->stack[DEFAULT_STACK_SIZE];
        Scheduler::putReadyThread(*handle);
    }
    else{
        (*handle)->stack = nullptr;
        (*handle)->context.ra = 0;
        (*handle)->context.sp = 0;
    }
    (*handle)->arg = arg;
    (*handle)->finished=false;
    (*handle)->timeSlice=DEFAULT_TIME_SLICE;
    (*handle)->timeSliceCounter = 0;
    if(!(*handle))return -1;
    return 0;
}   

void TCB::dispatch() {
    TCB* old = running;
    if(!old->isFinished()) { Scheduler::putReadyThread(old); }
    running = Scheduler::getReadyThread();

    TCB::contextSwitch(&old->context, &running->context);
}

void TCB::exit() {
    running->setFinished(true);
    delete running;
    running = nullptr;
    dispatch();
}

void TCB::threadWrapper(){
    RiscV::popSppSpie();
    running->body(running->arg);
    thread_exit();
}

TCB* TCB::createMain(){
    MemoryAllocator* memAlloc = MemoryAllocator::getInstance();
    TCB* t = (TCB*)memAlloc->mem_alloc(sizeof(TCB));
    if (!t) return nullptr;

    t->body   = nullptr;
    t->arg    = nullptr;
    t->stack  = nullptr;   
    t->finished = false;
    t->context.ra = (uint64)(&threadWrapper);
    t->context.sp = 0;
    t->timeSlice=DEFAULT_TIME_SLICE;
    t->timeSliceCounter = 0;

    return t;
}


uint64 TCB::getTimeSlice() const {
    return this->timeSlice;
}