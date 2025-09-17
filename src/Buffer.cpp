#include"../h/Buffer.hpp"

__Buffer::__Buffer(int capacity)
    : _capacity(capacity > 0 ? capacity : 1024),
      _buffer(new char[_capacity]),
      head(0),
      tail(0),
      spaceAvailable(new _Semaphore(_capacity)), 
      itemAvailable(new _Semaphore(0)),          
      mutexHead(new _Semaphore(1)),              
      mutexTail(new _Semaphore(1)){}

__Buffer::~__Buffer() {
    delete mutexTail;     mutexTail = nullptr;
    delete mutexHead;     mutexHead = nullptr;
    delete itemAvailable; itemAvailable = nullptr;
    delete spaceAvailable;spaceAvailable = nullptr;
    delete[] _buffer;     _buffer = nullptr;
}

int __Buffer::size() {
    int n;
    mutexHead->wait();
    mutexTail->wait();

    if (tail >= head) n = tail - head;
    else              n = _capacity - (head - tail);

    mutexTail->signal();
    mutexHead->signal();
    return n;
}


void __Buffer::put(char c) {
    spaceAvailable->wait();

    mutexTail->wait();
    _buffer[tail] = c;
    tail = (tail + 1) % _capacity;
    mutexTail->signal();

    itemAvailable->signal();
}

char __Buffer::get() {
    itemAvailable->wait();

    mutexHead->wait();
    char c = _buffer[head];
    head = (head + 1) % _capacity;
    mutexHead->signal();

    spaceAvailable->signal();
    return c;
}

int __Buffer::getCapacity() {
    return _capacity;
}
