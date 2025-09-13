#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "../lib/hw.h"
#include "../h/TCBList.hpp"
#include "../h/STQueue.hpp"

class TCB;

class Scheduler {
public:

    static void init();              

    static void putReadyThread(TCB* task);

    static TCB* getReadyThread(); 

    static void putSleepingThread(TCB* task, time_t sleepTime);

    static void awakeSleepingThread();

    static time_t getTime() { return time; };

    static void incTime() { time++; }

private:    

    static TCBList* readyThreadQueue;

    static STQueue* sleepThreadQueue;

    static time_t time;

};

#endif // SCHEDULER_HPP