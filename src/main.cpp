#include "../lib/console.h"
#include "../h/syscall_cpp.hpp"
#include "../h/syscall_c.hpp"
#include "../h/RiscV.hpp"
#include "../h/MemoryAllocator.hpp"
#include"../h/TCB.hpp"
#include"../h/Scheduler.hpp"



void _idle(void * a){
    while(true){
        __putc('i');
        Thread::dispatch();
    }
}


int main(){

    RiscV::w_stvec((uint64)&RiscV::supervisorTrap);

    TCB* _main = TCB::createMain();

    TCB::running = _main;

    Thread* idle = new Thread(_idle, nullptr);
    idle->start();

    while (true)
    {
        Thread::dispatch();
    }
    

    return 0;

}