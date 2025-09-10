#ifndef TCB_HPP
#define TCB_HPP

#include "syscall_c.hpp"
#include "../lib/hw.h"
#include "RiscV.hpp"

typedef void (*Body) (void*); 


class TCB {
public:
    static int createHandleTCB(TCB** handle, void (*) (void*), void* arg, void* stack = nullptr);

    static void dispatch();

    static void exit();

    static TCB *running;

    bool isFinished() const { return finished; }

    void setFinished(bool value) { finished = value; }

    static TCB* createMain();

    ~TCB();

    
private:

    struct Context{
        uint64 ra;
        uint64 sp;
    };

    Body body;

    void* arg;
    
    uint64 *stack;

    Context context;

    uint64 timeSlice;

    bool finished;

    friend class RiscV;

    friend int main();

    static void threadWrapper();

    static void contextSwitch(Context *oldContext, Context *runningContext);
};  
#endif // TCB_HPP