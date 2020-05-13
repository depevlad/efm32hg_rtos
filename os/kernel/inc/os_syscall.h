/*
 * os_syscall.h
 *
 *  Created on: Apr 15, 2020
 *      Author: depevlad
 */

#ifndef KERNEL_INC_OS_SYSCALL_H_
#define KERNEL_INC_OS_SYSCALL_H_

#include "../inc/os_msg.h"

typedef enum {
	SYS_YIELD,            /* Cede control to the next scheduled process. */
	SYS_EXIT,             /* Stop process. */
	SYS_SEND,             /* Send a message #1 to process #0. */
	SYS_SLEEP,            /* Sleep for #0 milliseconds. */
	SYS_PRINT,            /* Output the string pointed to by #0 over serial. */
	SYS_RECEIVE,          /* Receive a message from source #0 into address #1. */
	SYS_CONNECT_IRQ,      /* Register process as a handler for IRQ #0. */
	SYS_DISCONNECT_IRQ,   /* Free the process as a handler for IRQ #0. */
	SYS_ACKNOWLEDGE_IRQ,  /* Signal to the OS that IRQn #0 has been dealt with. */
} SystemCalls;

#define SYSCALL(n) __asm__ volatile ("svc %0" :: "i"(n));

void __attribute((noinline)) sysExit(void);
void __attribute((noinline)) sysYield(void);
void __attribute((noinline)) sysSleep(void);
void __attribute((noinline)) sysConnectIrq(uint8_t IRQn);
void __attribute((noinline)) sysDisconnectIrq(uint8_t IRQn);
void __attribute((noinline)) sysSend(uint8_t dst, Message *msg);
void __attribute((noinline)) sysReceive(uint8_t idSrc, Message *msg);
void __attribute((noinline)) sysAcknowledgeIrq(uint8_t IRQn);
void __attribute((noinline)) requestContextSwitch(void);

#endif /* KERNEL_INC_OS_SYSCALL_H_ */
