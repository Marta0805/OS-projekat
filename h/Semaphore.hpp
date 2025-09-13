#ifndef _SEMAPHORE_HPP
#define _SEMAPHORE_HPP


#include"../h/TCBList.hpp"
class TCB;


class _Semaphore{
public:
    _Semaphore(unsigned init = 1);

    ~_Semaphore();

    int wait();    

    void signal();

    static int createHandleSemaphore(_Semaphore** handle, unsigned init);

protected:

    void block();

    void deblock();

private:

    int val;

    TCBList blockedQueue;


};



#endif