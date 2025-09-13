#include "../h/Semaphore.hpp"
#include"../h/Scheduler.hpp"
#include"../h/TCB.hpp"
#include"../h/MemoryAllocator.hpp"

_Semaphore::_Semaphore(unsigned init) : val(init), blockedQueue() {}

_Semaphore::~_Semaphore(){
    TCB* blockedCurr = blockedQueue.pop_front();

    while(blockedCurr){
        blockedCurr->setBlocked(false);

        Scheduler::putReadyThread(blockedCurr);

        blockedCurr = blockedQueue.pop_front();
    }
}
int _Semaphore::wait(){
    //if(TCB::running->isBlocked()) return -1;

    if(--val<0) this->block();

    return 0;
}

void _Semaphore::signal(){

    if(val++ < 0) this->deblock();
}

void _Semaphore::block(){
    if(TCB::running == nullptr || TCB::running->isFinished() || TCB::running->isBlocked()) return;

    TCB::running->setBlocked(true);
    
    blockedQueue.push_back(TCB::running);

    TCB::dispatch();
}

void _Semaphore::deblock(){
    TCB* blocked = blockedQueue.pop_front();
    if(!blocked) return;

    blocked->setBlocked(false);

    Scheduler::putReadyThread(blocked);
}

int _Semaphore::createHandleSemaphore(_Semaphore** handle, unsigned init){
    MemoryAllocator* memAlloc = MemoryAllocator::getInstance();
    size_t blockSize = ((sizeof(_Semaphore) + MEM_BLOCK_SIZE - 1)) / MEM_BLOCK_SIZE;

    *handle = (_Semaphore*)memAlloc->mem_alloc(blockSize);
    if(!(*handle))return -1;
    (*handle)->val = init;
    (*handle)->blockedQueue.init();
    return 0;
}