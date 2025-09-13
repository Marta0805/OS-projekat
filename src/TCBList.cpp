#include "../h/TCBList.hpp"
#include "../lib/hw.h"
#include "../h/MemoryAllocator.hpp"

TCBList::TCBList() : head(nullptr), tail(nullptr), length(0) {}
TCBList::~TCBList() { clear(); }

void TCBList::push_back(TCB* value) {
    Node* n = allocNode();
    if (!n) return;
    n->data = value;
    n->next = nullptr;
    if (!tail) { head = tail = n; }
    else { tail->next = n; tail = n; }
    ++length;
}

void TCBList::push_front(TCB* value) {
    Node* n = allocNode();
    if (!n) return;
    n->data = value;
    n->next = head;
    head = n;
    if (!tail) tail = n;
    ++length;
}

TCB* TCBList::pop_front() {
    if (!head) return nullptr;
    Node* n = head;
    TCB* val = n->data;
    head = n->next;
    if (!head) tail = nullptr;
    freeNode(n);
    --length;
    return val;
}

bool TCBList::peekFront(TCB*& out) const {
    if (!head) return false;
    out = head->data;
    return true;
}

TCB* TCBList::front() const { return head ? head->data : nullptr; }
TCB* TCBList::back()  const { return tail ? tail->data : nullptr; }

int  TCBList::size()  const { return length; }
bool TCBList::empty() const { return length == 0; }

bool TCBList::removeFirstValue(TCB* value) {
    Node* prev = nullptr;
    Node* cur  = head;
    while (cur) {
        if (cur->data == value) {
            removeNode(prev, cur);
            return true;
        }
        prev = cur; cur = cur->next;
    }
    return false;
}

void TCBList::clear() {
    Node* cur = head;
    while (cur) {
        Node* nxt = cur->next;
        freeNode(cur);
        cur = nxt;
    }
    head = tail = nullptr;
    length = 0;
}

TCBList::Node* TCBList::allocNode() {
    size_t blocks = (sizeof(Node) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    return (Node*)MemoryAllocator::getInstance()->mem_alloc(blocks);
}

void TCBList::freeNode(Node* n) {
    if (n) MemoryAllocator::getInstance()->mem_free(n);
}

void TCBList::removeNode(Node* prev, Node* cur) {
    if (!prev) head = cur->next;
    else       prev->next = cur->next;
    if (!cur->next) tail = prev;
    freeNode(cur);
    --length;
}
