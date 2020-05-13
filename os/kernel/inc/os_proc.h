#ifndef KERNEL_INC_OS_PROC_H_
#define KERNEL_INC_OS_PROC_H_

#include "../inc/os_msg.h"

#define MAX_PROC_NAME 16
#define NUM_PROCS     16
#define MAX_PRIORITY  3

#define IDLE_ID 0

/* Set thumb bit. */
#define INIT_PSR 0x01000000

typedef enum {
	DEAD,
	READY,
	SENDING,
	RECEIVING,
	SLEEPING,
	IDLING
} ProcState;

typedef struct Process Process;

struct Process {
	char      pName[MAX_PROC_NAME];  /* Process name. */
	uint8_t   pId;                   /* Process ID. */
	ProcState pState;                /* Process state. */
	uint32_t *pSp;                   /* Process current stack pointer. */
	void     *pStk;                  /* Pointer to the bottom of the process stack. */
	uint32_t  pStkSize;              /* Process stack size. */
	uint32_t  pSleepTime;            /* Remaining sleep time. */
	uint32_t  pPending;              /* Contains the logical OR of pending interrupts. */
	uint8_t   pAccept;               /* Who the process can receive messages from. */
	uint8_t   pPriority;             /* Process priority. */
	Message  *pMsg;                  /* Pointer to message. */
	Process  *pWaiting;              /* Processes waiting to send to this one. */
	Process  *pNext;                 /* Process that is scheduled after this one. */
};

typedef struct {
	Process *qHead;
	Process *qTail;
} procQueue;

extern Process pTable[];
extern Process *crtProc;
extern Process *idleProc;

void makeReady(Process *p);
void chooseProcess(void);
void rescheduleProc(Process *p);

void fillPTable(Process *p, uint8_t pId, char *name, void *pStk, uint32_t stkSize);
void startProc(uint8_t pId, char *name, void (*pBody)(void), void *pStk, uint32_t stkSize);

void idle(void);
void startScheduler(void);

#endif /* KERNEL_INC_OS_PROC_H_ */
