
#ifndef RISCV_HPP
#define RISCV_HPP

#include "../lib/hw.h"

class RiscV
{
public:

    enum Scause{
        TIMER_INTERRUPT = (1ul << 63) | 1,
        CONSOLE_INTERRUPT = (1ul << 63) | 9,
        ILLEGAL_INSTRUCTION = 2,
        ILLEGAL_ADDRESS_READ = 5,
        ILLEGAL_ADDRESS_WRITE = 7,
        ECALL_USER_MODE= 8,
        ECALL_SUPERVISOR_MODE= 9
    };

    enum Syscall
    {
        MEM_ALLOC = 0x01,
        MEM_FREE = 0x02,
        MEM_GET_FREE_SPACE = 0x03,
        MEM_GET_LARGEST_FREE_BLOCK = 0x04,
        THREAD_CREATE = 0x11,
        THREAD_EXIT = 0x12,
        THREAD_DISPATCH = 0x13,
        SEM_OPEN = 0x21,
        SEM_CLOSE = 0x22,
        SEM_WAIT = 0x23,
        SEM_SIGNAL = 0x24,
        SEM_TIMEDWAIT = 0x25,
        SEM_TRYWAIT = 0x26,
        TIME_SLEEP = 0x31,
        GETC = 0x41,
        PUTC = 0x42
    };

    enum Registers{
        REG_ZERO = 0,
        REG_RA = 1,
        REG_SP = 2,
        REG_GP = 3,
        REG_TP = 4,
        REG_T0 = 5,
        REG_T1 = 6,
        REG_T2 = 7,
        REG_S0 = 8,
        REG_S1 = 9,
        REG_A0 = 10,
        REG_A1 = 11,
        REG_A2 = 12,
        REG_A3 = 13,
        REG_A4 = 14,
        REG_A5 = 15,
        REG_A6 = 16,
        REG_A7 = 17,
        REG_S2 = 18,
        REG_S3 = 19,
        REG_S4 = 20,
        REG_S5 = 21,
        REG_S6 = 22,
        REG_S7 = 23,
        REG_S8 = 24,
        REG_S9 = 25,
        REG_S10 = 26,
        REG_S11 = 27,
        REG_T3 = 28,
        REG_T4 = 29,
        REG_T5 = 30,
        REG_T6 = 31
    };

    // pop sstatus.spp and sstatus.spie bits
    static void popSppSpie();

    // read register scause
    static uint64 r_scause();

    // write register scause
    static void w_scause(uint64 scause);

    // read register sepc
    static uint64 r_sepc();

    // write register sepc
    static void w_sepc(uint64 sepc);

    // read register stvec
    static uint64 r_stvec();

    // write register stvec
    static void w_stvec(uint64 stvec);

    // read register stval
    static uint64 r_stval();

    // write register stval
    static void w_stval(uint64 stval);

    enum BitMaskSip
    {
        SIP_SSIP = (1 << 1),
        SIP_STIP = (1 << 5),
        SIP_SEIP = (1 << 9),
    };

    // mask set register sip
    static void ms_sip(uint64 mask);

    // mask clear register sip
    static void mc_sip(uint64 mask);

    // read register sip
    static uint64 r_sip();

    // write register sip
    static void w_sip(uint64 sip);

    enum BitMaskSstatus
    {
        SSTATUS_SIE = (1 << 1),
        SSTATUS_SPIE = (1 << 5),
        SSTATUS_SPP = (1 << 8),
    };

    // mask set register sstatus
    static void ms_sstatus(uint64 mask);

    // mask clear register sstatus
    static void mc_sstatus(uint64 mask);

    // read register sstatus
    static uint64 r_sstatus();

    // write register sstatus
    static void w_sstatus(uint64 sstatus);

    // supervisor trap
    static void supervisorTrap();

    static uint64 r_reg(uint64 reg);

    static void w_reg(uint64 reg, int value);



private:

    // supervisor trap handler
    static void handleSupervisorTrap();

};

inline uint64 RiscV::r_scause(){
    uint64 volatile scause;
    __asm__ volatile ("csrr %[scause], scause" : [scause] "=r"(scause));
    return scause;
}

inline void RiscV::w_scause(uint64 scause){
    __asm__ volatile ("csrw scause, %[scause]" : : [scause] "r"(scause));
}

inline uint64 RiscV::r_sepc(){
    uint64 volatile sepc;
    __asm__ volatile ("csrr %[sepc], sepc" : [sepc] "=r"(sepc));
    return sepc;
}

inline void RiscV::w_sepc(uint64 sepc){
    __asm__ volatile ("csrw sepc, %[sepc]" : : [sepc] "r"(sepc));
}

inline uint64 RiscV::r_stvec(){
    uint64 volatile stvec;
    __asm__ volatile ("csrr %[stvec], stvec" : [stvec] "=r"(stvec));
    return stvec;
}

inline void RiscV::w_stvec(uint64 stvec){
    __asm__ volatile ("csrw stvec, %[stvec]" : : [stvec] "r"(stvec));
}

inline uint64 RiscV::r_stval(){
    uint64 volatile stval;
    __asm__ volatile ("csrr %[stval], stval" : [stval] "=r"(stval));
    return stval;
}

inline void RiscV::w_stval(uint64 stval){
    __asm__ volatile ("csrw stval, %[stval]" : : [stval] "r"(stval));
}

inline void RiscV::ms_sip(uint64 mask){
    __asm__ volatile ("csrs sip, %[mask]" : : [mask] "r"(mask));
}

inline void RiscV::mc_sip(uint64 mask){
    __asm__ volatile ("csrc sip, %[mask]" : : [mask] "r"(mask));
}

inline uint64 RiscV::r_sip(){
    uint64 volatile sip;
    __asm__ volatile ("csrr %[sip], sip" : [sip] "=r"(sip));
    return sip;
}

inline void RiscV::w_sip(uint64 sip){
    __asm__ volatile ("csrw sip, %[sip]" : : [sip] "r"(sip));
}

inline void RiscV::ms_sstatus(uint64 mask){
    __asm__ volatile ("csrs sstatus, %[mask]" : : [mask] "r"(mask));
}

inline void RiscV::mc_sstatus(uint64 mask){
    __asm__ volatile ("csrc sstatus, %[mask]" : : [mask] "r"(mask));
}

inline uint64 RiscV::r_sstatus(){
    uint64 volatile sstatus;
    __asm__ volatile ("csrr %[sstatus], sstatus" : [sstatus] "=r"(sstatus));
    return sstatus;
}

inline void RiscV::w_sstatus(uint64 sstatus){
    __asm__ volatile ("csrw sstatus, %[sstatus]" : : [sstatus] "r"(sstatus));
}


inline uint64 RiscV::r_reg(uint64 reg) {
    uint64 volatile value;
    __asm__ volatile("ld %[value], 8*%[reg](fp)" : [value] "=r"(value) : [reg] "n"(reg));
    return value;
}

inline void RiscV::w_reg(uint64 reg, int value) {
    __asm__ volatile("sd %[value], 8*%[reg](fp)" : : [value] "r"(value), [reg] "n"(reg));
}
#endif // RISCV_HPP
