#include "../inc/os_msg.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../../drivers/io/inc/serial.h"

void sendMessage(uint8_t dst, Message *msg) {
	if (dst < 0 || dst > NUM_PROCS || pTable[dst].pState == DEAD)
		kPrintf("Sending to a process that is dead or does not exist!");

	uint8_t idSrc = crtProc->pId;
	msg->mSender  = idSrc;
	Process *pDst = &pTable[dst];

	if (pDst->pState == RECEIVING && (pDst->pAccept == ACCEPT_ANY || pDst->pAccept == idSrc)) {
		/* Receiver is waiting for us. */
		*(pDst->pMsg) = *msg;
		makeReady(pDst);
	} else {
		/* Sender must wait and join the receiver's queue. */
		crtProc->pState = SENDING;
		crtProc->pMsg   = msg;
		crtProc->pNext  = NULL;

		if (pDst->pWaiting == NULL) {
			pDst->pWaiting = crtProc;
		} else {
			/* Add sender to the back of the queue. */
			Process *q = pDst->pWaiting;
			while (q->pNext != NULL) q = q->pNext;
			q->pNext = crtProc;
		}

		chooseProcess();
	}
}

void receiveMessage(uint8_t idSrc, Message *msg) {
	if (idSrc != ACCEPT_ANY && idSrc != ACCEPT_HW &&
		(0 > idSrc || idSrc >= NUM_PROCS || pTable[idSrc].pState == DEAD)) {
		kPrintf("Trying to accept from nonexistent process %d.\n", idSrc);
	}

	/* See if any interrupts are pending. */
	if (crtProc->pPending && (idSrc == ACCEPT_ANY || idSrc == ACCEPT_HW)) {
		crtProc->pPending = 0;
		msg->mSender = ACCEPT_HW;
		msg->mType   = M_HARDWARE;
	}

	/* If not, pick the first suitable process from the queue. */
	Process *pPrev = NULL;
	for (Process *pSrc = crtProc->pWaiting; pSrc != NULL; pSrc = pSrc->pNext) {
		if (idSrc == ACCEPT_ANY || idSrc == pSrc->pId) {
			/* Receive from this process. */
			if (pPrev == NULL) {
				/* First process in the queue was suitable. */
				crtProc->pWaiting = pSrc->pNext;
			} else {
				pPrev->pNext = pSrc->pNext;
			}

			*msg = *(pSrc->pMsg);
			msg->mSender = pSrc->pId;

			makeReady(pSrc);

			return;
		}
		pPrev = pSrc;
	}

	/* Wait. */
	crtProc->pState  = RECEIVING;
	crtProc->pAccept = idSrc;
	crtProc->pMsg    = msg;
	chooseProcess();
}
