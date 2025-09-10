
#include"../h/MemoryAllocator.hpp"
#include"../h/syscall_cpp.hpp"
#include "../h/RiscV.hpp"
#include"../h/TCB.hpp"
#include"../h/Scheduler.hpp"

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
                break;
            }
            case MEM_FREE:{
                uint64 size = r_reg(11);
                ret=(uint64)MemoryAllocator::getInstance()->mem_free((void*)size);
                break;
            }
            case MEM_GET_FREE_SPACE:{
                ret=(uint64)MemoryAllocator::getInstance()->mem_get_free_space();
                break;
            }
            case MEM_GET_LARGEST_FREE_BLOCK:{
                ret=(uint64)MemoryAllocator::getInstance()->mem_get_largest_free_block();
                break;
            }
            case THREAD_CREATE:{
                thread_t* handle = (thread_t*)r_reg(11);
                void (*body)(void*) = (void (*)(void*))r_reg(12);
                void* arg = (void*)r_reg(13);
                void* stack = (void*)r_reg(14);

                ret = TCB::createHandleTCB((TCB**)handle, body, arg, stack);
                            
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

        }
        w_reg(10, ret);
        
    }
    
    w_sstatus(sstatus);
    w_sepc(sepc);
}