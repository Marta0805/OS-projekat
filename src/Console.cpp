#include "../h/Console.hpp"
#include "../h/syscall_cpp.hpp"
#include"../h/TCB.hpp"


__Buffer* _Console::putcBuffer = nullptr;
__Buffer* _Console::getcBuffer = nullptr;
_Semaphore*  _Console::getcSemaphore = nullptr;
_Semaphore* _Console::putcSemaphore = nullptr;
bool  _Console::isInterrupt = false;

_Console::_Console() {
    if (!getcSemaphore)  
        getcSemaphore = new _Semaphore(0);
    if(!putcSemaphore)
        putcSemaphore = new _Semaphore(0);
    if (!putcBuffer)     
        putcBuffer = new __Buffer((size_t)1024);
    if (!getcBuffer)     
        getcBuffer = new __Buffer((size_t)1024);

}

_Console* _Console::getInstance() {
    static _Console inst;
    return &inst;
}

void _Console::init() {
    static bool started = false;
    if (started) return;
    started = true;

    (void)getInstance();
}

bool _Console::getIsInterrupt() {
    return isInterrupt;
}


void _Console::_putc(char c) {
    if(putcBuffer->size() < putcBuffer->getCapacity()){
        putcBuffer->put(c);
        putcSemaphore->signal();
    }
    //inace vrati gresku
}

char _Console::_getc() {
    getcSemaphore->wait();
    return getcBuffer->get();
}


void _Console::_putcHandler(void*) {
    for (;;) {
        if(putcBuffer->size() == 0){
            putcSemaphore->wait();
        }
        uint8 cstatus = *((volatile uint8*)CONSOLE_STATUS);
        uint8 txReady = cstatus & CONSOLE_TX_STATUS_BIT;
        uint8 rxReady = cstatus & CONSOLE_RX_STATUS_BIT;
        if (txReady && isInterrupt) {
            char c = putcBuffer->get();
            *((volatile uint8*)CONSOLE_TX_DATA) = (uint8)c;
            break;
        }
        else if (!rxReady && !txReady && isInterrupt) {
            isInterrupt = false;
            plic_complete(CONSOLE_IRQ);  
        }
        else 
            TCB::dispatch();     
    }
}

void _Console::_getcHandler(void*) {
    for (;;) {
        while(getcBuffer->size() == getcBuffer->getCapacity())
            TCB::dispatch();

        uint8 cstatus = *((volatile uint8*)CONSOLE_STATUS);
        uint8 txReady = cstatus & CONSOLE_TX_STATUS_BIT;
        uint8 rxReady = cstatus & CONSOLE_RX_STATUS_BIT;

        if (rxReady && isInterrupt) {
            uint8 ch = *((volatile uint8*)CONSOLE_RX_DATA);
            getcBuffer->put((char)ch);
            getcSemaphore->signal(); 
        } else if (!rxReady && !txReady && isInterrupt) {
                isInterrupt = false;
                plic_complete(CONSOLE_IRQ);
        }
        else 
            TCB::dispatch();
    }
}


void _Console::console_handler() {
    isInterrupt = true;
}