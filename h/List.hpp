#ifndef LIST_HPP
#define LIST_HPP

#include "../lib/hw.h"
#include "../h/MemoryAllocator.hpp"

template<typename T>
class List {
public:
    List() : head(nullptr), tail(nullptr), lenght(0) {}
    ~List() { clear(); }

    List(const List&) = delete;
    List& operator=(const List&) = delete;

    void push_back(const T& value) {
        size_t blockSize = ((sizeof(Node) + MEM_BLOCK_SIZE - 1 + MemoryAllocator::HEADER_BLOCKS)) / MEM_BLOCK_SIZE;
        Node* n = (Node*)MemoryAllocator::getInstance()->mem_alloc(blockSize);
        n->data = value;
        n->next = nullptr;
        if (!tail) { head = tail = n; }
        else { tail->next = n; tail = n; }
        ++lenght;
    }

    void push_front(const T& value) {
        size_t blockSize = ((sizeof(Node) + MEM_BLOCK_SIZE - 1 + MemoryAllocator::HEADER_BLOCKS)) / MEM_BLOCK_SIZE;
        Node* n = (Node*)MemoryAllocator::getInstance()->mem_alloc(blockSize);
        n->data = value;
        n->next = head;
        head = n;
        if (!tail) tail = n;
        ++lenght;
    }

    T pop_front() {
        if (!head) return T{};
        Node* n = head;
        T val = n->data;
        head = n->next;
        if (!head) tail = nullptr;
        MemoryAllocator::getInstance()->mem_free(n);
        --lenght;
        return val;
    }

    bool peekFront(T& out) {
        if (!head) return false;
        Node* n = head;
        out = n->data;
        head = n->next;
        if (!head) tail = nullptr;
        MemoryAllocator::getInstance()->mem_free(n);
        --lenght;
        return true;
    }


    T front() const { 
        return head ? head->data : T{}; 
    }
    T back()  const { 
        return tail ? tail->data : T{}; 
    }

    int  size()  const { 
        return lenght; 
    }

    bool empty() const { 
        return lenght == 0;
    }

    bool removeFirstValue(const T& value) {
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

    template<typename Pred>
    bool removeCond(Pred pred) {
        Node* prev = nullptr;
        Node* cur  = head;
        while (cur) {
            if (pred(cur->data)) {
                removeNode(prev, cur);
                return true;
            }
            prev = cur; cur = cur->next;
        }
        return false;
    }

    template<typename Compare>
    void insertSorted(const T& value, Compare comp) {
        size_t blockSize = ((sizeof(Node) + MEM_BLOCK_SIZE - 1 + MemoryAllocator::HEADER_BLOCKS)) / MEM_BLOCK_SIZE;
        Node* n = (Node*)MemoryAllocator::getInstance()->mem_alloc(blockSize);

        if (!head || comp(value, head->data)) {
            n->next = head;
            head = n;
            if (!tail) tail = n;
            ++lenght;
            return;
        }

        Node* prev = head;
        Node* cur  = head->next;
        while (cur && !comp(value, cur->data)) {
            prev = cur; cur = cur->next;
        }

        prev->next = n;
        n->next = cur;
        if (!cur) tail = n;
        ++lenght;
    }

    void clear() {
        Node* cur = head;
        while (cur) {
            Node* nxt = cur->next;
            MemoryAllocator::getInstance()->mem_free(cur);
            cur = nxt;
        }
        head = tail = nullptr;
        lenght = 0;
    }

private:

    struct Node {
        T data;
        Node* next;
        explicit Node(const T& d) : data(d), next(nullptr) {}
    };

    void removeNode(Node* prev, Node* cur) {
        if (!prev) head = cur->next;
        else       prev->next = cur->next;
        if (!cur->next) tail = prev;
        MemoryAllocator::getInstance()->mem_free(cur);
        --lenght;
    }

    Node* head;
    Node* tail;
    int   lenght;
};


#endif // LIST_HPP