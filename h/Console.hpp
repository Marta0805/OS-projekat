#ifndef _CONSOLE_HPP_
#define _CONSOLE_HPP_

#include "../h/Semaphore.hpp"
#include "../h/Buffer.hpp"

class _Console {
public:
    static _Console* getInstance();

    static void init();

    static bool getIsInterrupt();

    static void _putc(char c);

    static char _getc();

    static void _putcHandler(void*);

    static void _getcHandler(void*);

    static _Semaphore* getcSemaphore;

    static _Semaphore* putcSemaphore;

    void console_handler();

     _Console(_Console& other) = delete;
    void operator=(const _Console&) = delete;

private:
    _Console();

    static __Buffer* putcBuffer;

    static __Buffer* getcBuffer;

    static bool isInterrupt;
};



#endif //_CONSOLE_HPP_
