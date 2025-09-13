#include "../h/MemoryAllocator.hpp"
#include "../h/STQueue.hpp"



void STQueue::pushSTQ(SleepingNode* data) {
    if (!data) return;

    size_t nBlocks = (sizeof(Node) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    Node* newNode = (Node*)MemoryAllocator::getInstance()->mem_alloc(nBlocks);
    newNode->data = data;
    newNode->next = nullptr;

    if (!head) {
        head = tail = newNode;
        ++length;
        return;
    }

    Node* prev = nullptr;
    Node* curr = head;

    while (curr && curr->data->wakeUpTime < data->wakeUpTime) {
        prev = curr;
        curr = curr->next;
    }

    if (!prev) {
        newNode->next = head;
        head = newNode;
        if (!tail) tail = newNode;
    } else {
        prev->next = newNode;
        newNode->next = curr;
        if (!curr) tail = newNode;
    }

    ++length;
}

SleepingNode* STQueue::popSTQ() {
    if (!head) return nullptr;

    Node* n = head;
    head = head->next;
    if (!head) tail = nullptr;

    SleepingNode* sn = n->data;
    MemoryAllocator::getInstance()->mem_free(n);
    --length;
    return sn;
}

SleepingNode* STQueue::front() const {
    return head ? head->data : nullptr;
}

SleepingNode* STQueue::back() const {
    return tail ? tail->data : nullptr;
}

bool STQueue::removeSTQ(TCB* tcb) {
    if (!tcb) return false;

    Node* prev = nullptr;
    Node* curr = head;

    while (curr) {
        if (curr->data && curr->data->tcb == tcb) {
            if (!prev) head = curr->next;
            else       prev->next = curr->next;

            if (curr == tail) tail = prev;

            MemoryAllocator::getInstance()->mem_free(curr);
            --length;
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
}

void STQueue::init(){
    head = nullptr;
    tail = nullptr;
    length = 0;
}
