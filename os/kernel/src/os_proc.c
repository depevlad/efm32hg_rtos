#include "../inc/os_proc.h"

#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../../drivers/io/inc/serial.h"
#include "../../tasks/idle/inc/idle.h"

#include "../inc/os_exceptions.h"
#include "../inc/os_syscall.h"
#include "../inc/os_mem.h"

Process pTable[NUM_PROCS];
Process *crtProc;
Process *idleProc;

void fillPTable(Process *p, uint8_t pId, char *name, void *pStk, uint32_t stkSize) {
	p->pId        = pId;
	p->pState     = READY;
	p->pSp        = (uint32_t*)(pStk + stkSize);
	p->pStk       = pStk;
	p->pStkSize   = stkSize;
	p->pSleepTime = 0;
	p->pWaiting   = NULL;
	p->pPending   = 0;
	p->pPriority  = 0;
	p->pNext      = NULL;
	p->pAccept    = ACCEPT_ANY;
	p->pMsg       = NULL;
	p->pName[MAX_PROC_NAME - 1] = 0;
	strncpy(p->pName, name, MAX_PROC_NAME - 1);
	/* Clear the stack. */
	memset(pStk, 0, stkSize);
}

void startProc(uint8_t pId, char *name, void (*pBody)(void), void *pStk, uint32_t stkSize) {
	if (pId == 0) {
		/* Start idle task. */
		idleProc = &pTable[IDLE_ID];
		fillPTable(idleProc, 0, "idle", pStk, stkSize);
		idleProc->pState    = IDLING;
		idleProc->pPriority = MAX_PRIORITY;
	} else {
		Process *p = &pTable[pId];

		if (p->pState != DEAD)
			kPrintf("pId for process already taken! (%d)\n", pId);

		fillPTable(p, pId, name, pStk, stkSize);

		/* Create a fake exception frame on the process stack. */
		uint32_t *new_pSp = p->pSp - 16;
		memset(new_pSp, 0, 64);
		new_pSp[SAVED_PSR] = INIT_PSR;
		new_pSp[SAVED_PC]  = (uint32_t)pBody;
		new_pSp[SAVED_LR]  = (uint32_t)exit;
		p->pSp = new_pSp;

		makeReady(p);
	}
}

/* Set PSP to @pSp and enter thread mode. */
void __attribute((naked)) enterThreadMode(uint32_t *pSp) {
	__asm__ volatile (
		"msr psp, r0      \n"
		"mov r0, #0x2     \n"
		"msr control, r0  \n"
		"isb              \n"
		"bx lr            \n"
	);
}

void startScheduler(void) {
	crtProc = idleProc;
	enterThreadMode(crtProc->pSp);
	sysYield();
	idle();
}

/* ========== PROCESS QUEUES ========== */

procQueue readyQueue[MAX_PRIORITY];

void rescheduleProc(Process *p) {
	procQueue *q = &readyQueue[p->pPriority];

	/* Delete p from its queue. */
	if (q->qHead != NULL) {
		Process *prev = NULL;
		while (q->qHead != p) {
			prev = q->qHead;
			q->qHead = q->qHead->pNext;
		}
		if (prev != NULL)
			prev->pNext = p->pNext;
	}

	p->pPriority = 0;
	makeReady(p);
}

void makeReady(Process *p) {
	if (p != idleProc) {
		p->pState = READY;
		p->pNext  = NULL;
		procQueue *q = &readyQueue[p->pPriority];
		if (q->qHead == NULL)
			q->qHead = p;
		else
			q->qTail->pNext = p;
		q->qTail = p;
	}
}

void chooseProcess(void) {
	for (uint8_t pLevel = 0; pLevel < MAX_PRIORITY; pLevel += 1) {
		procQueue *q = &readyQueue[pLevel];
		if (q->qHead != NULL) {
			crtProc  = q->qHead;
			q->qHead = q->qHead->pNext;
			return;
		}
	}
	crtProc = idleProc;
}
