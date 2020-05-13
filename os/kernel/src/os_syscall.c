#include "../inc/os_syscall.h"

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "em_core.h"

#include "../../drivers/io/inc/serial.h"
#include "../inc/os_irq.h"
#include "../inc/os_proc.h"
#include "../inc/os_exceptions.h"

#define INSTR_SIZE 2

uint32_t *syscallHandler(uint32_t *pSp) {
	uint8_t  svcId = ((uint8_t*)(pSp[SAVED_PC]))[-(INSTR_SIZE)];
	uint32_t arg   = pSp[SAVED_R0];
	Message *msg   = (Message*)pSp[SAVED_R1];
	crtProc->pSp = pSp;

	switch(svcId) {
		case SYS_EXIT:
			crtProc->pState = DEAD;
			chooseProcess();
			break;
		case SYS_YIELD:
			makeReady(crtProc);
			chooseProcess();
			break;
		case SYS_CONNECT_IRQ:
			irqConnect(arg);
			break;
		case SYS_DISCONNECT_IRQ:
			irqDisconnect(arg);
			break;
		case SYS_ACKNOWLEDGE_IRQ:
			irqAcknowledge(arg);
			break;
		case SYS_SEND:
			sendMessage(arg, msg);
			break;
		case SYS_RECEIVE:
			receiveMessage(arg, msg);
			break;
		case SYS_PRINT:
			kPrintf(arg);
			break;
		case SYS_SLEEP:
			crtProc->pSleepTime = arg;
			crtProc->pState = SLEEPING;
			chooseProcess();
			break;
		default:
			kPrintf("Invalid SVC %d!\n", svcId);
			break;
	}

	return crtProc->pSp;
}

void __attribute((noinline)) sysExit(void)                           { SYSCALL(SYS_EXIT); }
void __attribute((noinline)) sysYield(void)                          { SYSCALL(SYS_YIELD); }
void __attribute((noinline)) sysSleep(void)                          { SYSCALL(SYS_SLEEP); }
void __attribute((noinline)) sysConnectIrq(uint8_t IRQn)             { SYSCALL(SYS_CONNECT_IRQ); }
void __attribute((noinline)) sysDisconnectIrq(uint8_t IRQn)          { SYSCALL(SYS_DISCONNECT_IRQ); }
void __attribute((noinline)) sysAcknowledgeIrq(uint8_t IRQn)         { SYSCALL(SYS_ACKNOWLEDGE_IRQ); }
void __attribute((noinline)) sysSend(uint8_t dst, Message *msg)      { SYSCALL(SYS_SEND); }
void __attribute((noinline)) sysReceive(uint8_t idSrc, Message *msg) { SYSCALL(SYS_RECEIVE); }

/* ========== CONTEXT SWITCHING ========== */

void __attribute((noinline)) requestContextSwitch(void)  { SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk; }

uint32_t *cxtSwitchHandler(uint32_t *pSp) {
	crtProc->pSp = pSp;
	makeReady(crtProc);
	chooseProcess();
	return crtProc->pSp;
}
