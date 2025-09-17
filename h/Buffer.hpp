#ifndef _BUFFER_HPP_
#define _BUFFER_HPP_

#include"../h/Semaphore.hpp"

class __Buffer{

public:
    __Buffer(int capacity = 1024);
    ~__Buffer();

    void put(char c);
    char get();

    int getCapacity();

    int getHead() {return head;}

    int getTail() {return tail;}

    int size();


private:
    int _capacity;

    char* _buffer;
    int head, tail;


    _Semaphore* spaceAvailable;
    _Semaphore* itemAvailable;
    _Semaphore* mutexHead;
    _Semaphore* mutexTail;
};


#endif 