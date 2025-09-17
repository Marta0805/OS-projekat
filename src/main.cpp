#include "../lib/console.h"
#include "../h/syscall_cpp.hpp"
#include "../h/syscall_c.hpp"
#include "../h/RiscV.hpp"
#include "../h/MemoryAllocator.hpp"
#include"../h/TCB.hpp"
#include"../h/Scheduler.hpp"
#include"../h/Console.hpp"
#include "../h/Semaphore.hpp"


extern void userMain();

void userMainWrapper(void* arg) {
    Semaphore* sem = (Semaphore*)arg;
    userMain();

    sem->signal();
}


void _idle(void * a){
    while(true){
        Thread::dispatch();
    }
}

int main(){
    RiscV::w_stvec((uint64)&RiscV::supervisorTrap);

    Scheduler::init();
    _Console::init();

    _Console* console = _Console::getInstance();
    

    Thread* tx = new Thread(console->_putcHandler, nullptr);
    Thread* rx = new Thread(console->_getcHandler, nullptr);

    tx->start();

    rx->start();

    TCB* _main = TCB::createMain();

    TCB::running = _main;

    Thread* idle = new Thread(_idle, nullptr);

    idle->start();
    
    Semaphore* sem = new Semaphore(0);
    
    Thread* userThread = new Thread(userMainWrapper, sem);

    userThread->start();
    
    sem->wait(); 

    delete sem;

    return 0;
}