#include"../h/syscall_cpp.hpp"

#include"../h/TCBList.hpp"
#include"../h/Scheduler.hpp"
#include"../h/TCB.hpp"
#include "../h/MemoryAllocator.hpp"

#include"../lib/console.h"


TCBList* Scheduler::readyThreadQueue = nullptr;

STQueue* Scheduler::sleepThreadQueue = nullptr;

uint64 Scheduler::time = 0;

void Scheduler::init() {
    size_t blockSize = ((sizeof(TCBList) + MEM_BLOCK_SIZE - 1)) / MEM_BLOCK_SIZE;

    readyThreadQueue = (TCBList*)MemoryAllocator::getInstance()->mem_alloc(blockSize);

    blockSize = ((sizeof(STQueue) + MEM_BLOCK_SIZE - 1)) / MEM_BLOCK_SIZE;

    sleepThreadQueue = (STQueue*)MemoryAllocator::getInstance()->mem_alloc(blockSize);

    readyThreadQueue->init();
    sleepThreadQueue->init();

    time = 0;
}

void Scheduler::putReadyThread(TCB* task) {
    readyThreadQueue->push_back(task);
    task->setReady(true);
}
TCB* Scheduler::getReadyThread() {
    TCB* task = readyThreadQueue->pop_front();
    return task;
}

void Scheduler::putSleepingThread(TCB* task, time_t wakeUptime){
    size_t blockSize = ((sizeof(SleepingNode) + MEM_BLOCK_SIZE - 1)) / MEM_BLOCK_SIZE;
    SleepingNode* node = (SleepingNode*)MemoryAllocator::getInstance()->mem_alloc(blockSize);
    node->tcb = task;
    node->wakeUpTime = Scheduler::time + wakeUptime;
    task->setReady(false);
    sleepThreadQueue->pushSTQ(node);
}

void Scheduler::awakeSleepingThread(){
    while (sleepThreadQueue && !sleepThreadQueue->empty()) {
        SleepingNode* sn = sleepThreadQueue->front();   
        if (!sn || sn->wakeUpTime > Scheduler::time) break;  
        sn = sleepThreadQueue->popSTQ();    
        putReadyThread(sn->tcb);                                
        MemoryAllocator::getInstance()->mem_free(sn);   
    }

}
