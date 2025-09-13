#include "../lib/console.h"
#include "../h/syscall_cpp.hpp"
#include "../h/syscall_c.hpp"
#include "../h/RiscV.hpp"
#include "../h/MemoryAllocator.hpp"
#include"../h/TCB.hpp"
#include"../h/Scheduler.hpp"


extern void userMain();

void userMainWrapper(void* arg) {
    userMain();
}


void _idle(void * a){
    while(true){
        Thread::dispatch();
    }
}


int main(){
    
    Scheduler::init();

    RiscV::w_stvec((uint64)&RiscV::supervisorTrap);

    TCB* _main = TCB::createMain();

    TCB::running = _main;

    Thread* idle = new Thread(_idle, nullptr);

    idle->start();

    Thread* userThread = new Thread(userMainWrapper, nullptr);

    userThread->start();

    TCB::dispatch();

    return 0;


}