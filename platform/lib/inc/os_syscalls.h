#include "os_msg.h"

typedef enum {
    SYS_YIELD,
    SYS_EXIT,
    SYS_SEND,
    SYS_SLEEP,
    SYS_PRINT,
    SYS_RECEIVE,
    SYS_CONNECT_IRQ,
    SYS_DISCONNECT_IRQ,
    SYS_ACKNOWLEDGE_IRQ,
} SystemCalls;

#define SYSCALL(n) __asm__ volatile ("svc %0" :: "i"(n));

void __attribute((noinline)) sysExit(void)                           { SYSCALL(SYS_EXIT); }
void __attribute((noinline)) sysYield(void)                          { SYSCALL(SYS_YIELD); }
void __attribute((noinline)) sysSleep(uint32_t mSec)                 { SYSCALL(SYS_SLEEP); }
void __attribute((noinline)) sysPrint(char *fmt)                     { SYSCALL(SYS_PRINT); }
void __attribute((noinline)) sysConnectIrq(uint8_t IRQn)             { SYSCALL(SYS_CONNECT_IRQ); }
void __attribute((noinline)) sysDisconnectIrq(uint8_t IRQn)          { SYSCALL(SYS_DISCONNECT_IRQ); }
void __attribute((noinline)) sysAcknowledgeIrq(uint8_t IRQn)         { SYSCALL(SYS_ACKNOWLEDGE_IRQ); }
void __attribute((noinline)) sysSend(uint8_t dst, Message *msg)      { SYSCALL(SYS_SEND); }
void __attribute((noinline)) sysReceive(uint8_t idSrc, Message *msg) { SYSCALL(SYS_RECEIVE); }