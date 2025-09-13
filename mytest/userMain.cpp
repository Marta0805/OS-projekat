#include "../h/syscall_cpp.hpp"

#include"../h/print.hpp"


static volatile bool finishedA = false;
static volatile bool finishedB = false;
static volatile bool finishedC = false;
static volatile bool finishedD = false;

static uint64 fibonacci(uint64 n) {
    if (n == 0 || n == 1) { return n; }
    if (n % 10 == 0) { thread_dispatch(); }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

static void workerBodyA(void* arg) {
    for (uint64 i = 0; i < 10; i++) {
        for (uint64 j = 0; j < 10000; j++) {
            for (uint64 k = 0; k < 30000; k++) { /* busy wait */ }
            thread_dispatch();
        }
    }
    finishedA = true;
}

static void workerBodyB(void* arg) {
    for (uint64 i = 0; i < 16; i++) {
        for (uint64 j = 0; j < 10000; j++) {
            for (uint64 k = 0; k < 30000; k++) { /* busy wait */ }
            thread_dispatch();
        }
    }
    finishedB = true;
    thread_dispatch();
}

static void workerBodyC(void* arg) {
    uint8 i = 0;
    for (; i < 3; i++) {
    }

    __asm__ ("li t1, 7");
    thread_dispatch();

    uint64 t1 = 0;
    __asm__ ("mv %[t1], t1" : [t1] "=r"(t1));


    fibonacci(12);

    for (; i < 6; i++) {

    }

    finishedC = true;
    thread_dispatch();
}

static void workerBodyD(void* arg) {
    uint8 i = 10;
    for (; i < 13; i++) {
    }

    __asm__ ("li t1, 5");
    thread_dispatch();

    fibonacci(16);


    for (; i < 16; i++) {
    }

    finishedD = true;
    thread_dispatch();
}


void userMain() {
    thread_t threads[4];
    thread_create(&threads[0], workerBodyA, nullptr);

    thread_create(&threads[1], workerBodyB, nullptr);

    thread_create(&threads[2], workerBodyC, nullptr);

    thread_create(&threads[3], workerBodyD, nullptr);


    while (!(finishedA && finishedB && finishedC && finishedD)) {
        thread_dispatch();
    }

}
