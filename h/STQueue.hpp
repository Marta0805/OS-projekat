#ifndef _SLEEP_QUEUE_HPP
#define _SLEEP_QUEUE_HPP

#include "../h/syscall_c.hpp"

class TCB;

struct SleepingNode {
    TCB*   tcb;
    time_t wakeUpTime;
};

class STQueue {
public:
    STQueue() : head(nullptr), tail(nullptr), length(0) {}

    void pushSTQ(SleepingNode* data);

    SleepingNode* popSTQ();

    SleepingNode* front() const;
    SleepingNode* back()  const;

    void init();

    bool removeSTQ(TCB* tcb);

    int  size()   const { return length; }
    bool empty()  const { return length == 0; }

private:
    struct Node {
        SleepingNode* data;
        Node*         next;
    };

    Node* head;
    Node* tail;
    int   length;

    STQueue(const STQueue&) = delete;
    STQueue& operator=(const STQueue&) = delete;
};

#endif 