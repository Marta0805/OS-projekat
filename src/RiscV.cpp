
#include"../h/MemoryAllocator.hpp"
#include"../h/syscall_cpp.hpp"
#include "../h/RiscV.hpp"
#include"../h/TCB.hpp"
#include"../h/Scheduler.hpp"
#include "../lib/console.h"
#include"../h/Semaphore.hpp"

void RiscV::popSppSpie(){
    __asm__ volatile("csrc sstatus, %0" ::"r"(SSTATUS_SPP));
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void RiscV::handleSupervisorTrap(){
    volatile uint64 scause = r_scause();
    volatile uint64 sepc = r_sepc();
    volatile uint64 sstatus = r_sstatus();
    uint64 ret=0;
    

    if(scause == ECALL_USER_MODE || scause == ECALL_SUPERVISOR_MODE){
        sepc=sepc+4;

        uint64 a0 = r_reg(10); 
        switch(a0){
            case MEM_ALLOC:{
                uint64 size = r_reg(11);
                ret=(uint64)MemoryAllocator::getInstance()->mem_alloc((size_t)size);
                w_reg(10, ret);
                break;
            }
            case MEM_FREE:{
                uint64 size = r_reg(11);
                ret=(uint64)MemoryAllocator::getInstance()->mem_free((void*)size);
                w_reg(10, ret);
                break;
            }
            case MEM_GET_FREE_SPACE:{
                ret=(uint64)MemoryAllocator::getInstance()->mem_get_free_space();
                w_reg(10, ret);
                break;
            }
            case MEM_GET_LARGEST_FREE_BLOCK:{
                ret=(uint64)MemoryAllocator::getInstance()->mem_get_largest_free_block();
                w_reg(10, ret);
                break;
            }
            case THREAD_CREATE:{
                thread_t* handle = (thread_t*)r_reg(11);
                void (*body)(void*) = (void (*)(void*))r_reg(12);
                void* arg = (void*)r_reg(13);
                void* stack = (void*)r_reg(14);

                ret = TCB::createHandleTCB((TCB**)handle, body, arg, stack);
                w_reg(10, ret);     
                break;
            }
            case THREAD_EXIT:{
                TCB::exit();
                break;
            }
            case THREAD_DISPATCH:{
                TCB::dispatch();    
                break;
            }
            case SEM_OPEN:{
                _Semaphore** handle = (_Semaphore**)r_reg(11);
                unsigned init = (unsigned) r_reg(12);
                ret = _Semaphore::createHandleSemaphore((_Semaphore**) handle, init);
                
                w_reg(10, ret);
                break;
            }
            case SEM_CLOSE:{
                _Semaphore* handle = (_Semaphore*)r_reg(11);
                (handle)->~_Semaphore();
                ret = MemoryAllocator::getInstance()->mem_free(handle);
                if(ret == 0) w_reg(10, 0);
                else w_reg(10, -1);
                break;
            }
            case SEM_WAIT:{
                _Semaphore* sem = (_Semaphore*)r_reg(11);
                if(sem != nullptr){
                    ret = sem->wait();
                    w_reg(10, ret);
                    break;
                }
                else w_reg(10, -1);
                break;
            }
            case SEM_SIGNAL:{
                _Semaphore* sem = (_Semaphore*)r_reg(11);
                if(sem != nullptr){
                    sem->signal();
                    w_reg(10, 0);
                    break;
                }
                else w_reg(10, -1);
                break;
            }
            case TIME_SLEEP:{
                time_t time = (time_t)r_reg(11);
                Scheduler::putSleepingThread(TCB::running, time);
                TCB::dispatch();
                w_reg(10, 0);
                break;
            }

        }
        
    }
    else if(scause==TIMER_INTERRUPT){
        clear_mask_sip(SIP_SSIE);

        Scheduler::incTime();
        Scheduler::awakeSleepingThread();

        TCB::running->timeSliceCounter++;
        if (TCB::running->timeSliceCounter >= TCB::running->getTimeSlice()) {
            TCB::running->timeSliceCounter = 0;
            TCB::dispatch();
        }
    }
    else if(scause==CONSOLE_INTERRUPT)
    {
        console_handler();
    }
    else
    {
        uint32 val = 0x5555;
        uint64 addr = 0x100000;
        __asm__ volatile("sw %[val], 0(%[addr])" : : [val] "r"(val), [addr] "r"(addr));
        while (1);
    }
    
    w_sstatus(sstatus);
    w_sepc(sepc);
}