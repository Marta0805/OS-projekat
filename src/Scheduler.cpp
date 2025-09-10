#include"../h/syscall_cpp.hpp"

#include"../h/List.hpp"
#include"../h/Scheduler.hpp"
#include"../h/TCB.hpp"
#include "../h/MemoryAllocator.hpp"

#include"../lib/console.h"

Scheduler* Scheduler::instance = nullptr;

Scheduler::Scheduler() : readyThreadQueue(){};

Scheduler* Scheduler::getInstance() {
    if(!instance){
        size_t blockSize = ((sizeof(List<TCB*>) + MEM_BLOCK_SIZE - 1 + MemoryAllocator::HEADER_BLOCKS)) / MEM_BLOCK_SIZE;
        instance = (Scheduler*)MemoryAllocator::getInstance()->mem_alloc(blockSize);
    }
    return instance;
}

void Scheduler::putReadyThread(TCB* task) {
    readyThreadQueue.push_back(task);
}
TCB* Scheduler::getReadyThread() {
    TCB* task = readyThreadQueue.pop_front();
    return task;
}
