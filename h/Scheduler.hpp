#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "../lib/hw.h"
#include "../h/List.hpp"

class TCB;



class Scheduler {
public:
    static Scheduler* getInstance();

    void putReadyThread(TCB* task);

    TCB* getReadyThread(); 

private:
    Scheduler();
    
    static Scheduler* instance;

    List<TCB*> readyThreadQueue;

    Scheduler(const Scheduler&) = delete;
    Scheduler& operator=(const Scheduler&) = delete;    
};

#endif // SCHEDULER_HPP