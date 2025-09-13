#ifndef TCB_LIST_HPP
#define TCB_LIST_HPP

class TCB;

class TCBList {
public:
    TCBList();
    ~TCBList();

    TCBList(const TCBList&) = delete;

    void push_back(TCB* value);
    void push_front(TCB* value);
    TCB* pop_front();

    bool peekFront(TCB*& out) const;

    TCB* front() const;
    TCB* back() const;

    int  size() const;
    bool empty() const;

    bool removeFirstValue(TCB* value);
    void clear();

    void init(){
        head = nullptr;
        tail = nullptr;
        length = 0;
    }


private:
    struct Node {
        TCB*  data;
        Node* next;
    };

    static Node* allocNode();
    static void  freeNode(Node* n);
    void removeNode(Node* prev, Node* cur);

    Node* head;
    Node* tail;
    int   length;
};

#endif // TCB_LIST_HPP
