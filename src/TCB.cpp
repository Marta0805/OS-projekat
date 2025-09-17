#include"../h/TCB.hpp"
#include"../h/RiscV.hpp"
#include"../h/syscall_c.hpp"
#include"../lib/hw.h"
#include"../h/Scheduler.hpp"
#include"../h/MemoryAllocator.hpp"

#include "../lib/console.h"
#include"../h/Console.hpp"

TCB* TCB::running = nullptr;


TCB::~TCB() {
    if(stack){
        MemoryAllocator::getInstance()->mem_free(stack);
    };
}   


int TCB::createHandleTCB(TCB** handle, Body body, void* arg, void* stack) {
    MemoryAllocator* memAlloc = MemoryAllocator::getInstance();
    size_t blockSize = ((sizeof(TCB) + MEM_BLOCK_SIZE - 1)) / MEM_BLOCK_SIZE;
    *handle = (TCB*)memAlloc->mem_alloc(blockSize);
    (*handle)->body = body;
    if(body == _Console::_getcHandler || body == _Console::_putcHandler){
        (*handle)->stack = (uint64*)stack;
        (*handle)->context.ra = body==_Console::_getcHandler? (uint64)(&_Console::_getcHandler) : (uint64)(&_Console::_putcHandler);
        (*handle)->context.sp = (uint64)&(*handle)->stack[DEFAULT_STACK_SIZE];
        Scheduler::putReadyThread(*handle);
    }
    else if(body){
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
    (*handle)->blocked = false;
    (*handle)->ready = true;
    (*handle)->timeSlice=DEFAULT_TIME_SLICE;
    (*handle)->timeSliceCounter = 0;
    if(!(*handle))return -1;
    return 0;
}   

void TCB::dispatch() {
    TCB* old = running;
    if(!old->isFinished() && old->isReady()) { Scheduler::putReadyThread(old); }
    else if(old->isFinished()){
        delete old;
    }
    running = Scheduler::getReadyThread();

    // if(old!=running && 
    //     (old->body!=_Console::_getcHandler &&  old->body != _Console::_putcHandler))

    if(old != running && running)
        TCB::contextSwitch(&old->context, &running->context);
}

void TCB::exit() {
    if(running->body)
        running->setFinished(true);
    dispatch();
}

void TCB::threadWrapper(){
    RiscV::popSppSpie();
    running->body(running->arg);
    thread_exit();
}

TCB* TCB::createMain(){
    MemoryAllocator* memAlloc = MemoryAllocator::getInstance();
    size_t blockSize = ((sizeof(TCB) + MEM_BLOCK_SIZE - 1)) / MEM_BLOCK_SIZE;
    TCB* t = (TCB*)memAlloc->mem_alloc(sizeof(blockSize));
    if (!t) return nullptr;

    t->body   = nullptr;
    t->arg    = nullptr;
    t->stack  = nullptr;   
    t->finished = false;
    t->blocked = false;
    t->ready = false;
    t->context.ra = (uint64)(&threadWrapper);
    t->context.sp = 0;
    t->timeSlice=DEFAULT_TIME_SLICE;
    t->timeSliceCounter = 0;

    return t;
}


uint64 TCB::getTimeSlice() const {
    return this->timeSlice;
}