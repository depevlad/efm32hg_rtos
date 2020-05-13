#include "../inc/os_irq.h"
#include "../inc/os_proc.h"
#include "../inc/os_syscall.h"

#include "em_core.h"

#include "../../drivers/io/inc/serial.h"

uint8_t irqHandler[NUM_IRQS];

void irqConnect(uint8_t IRQn) {
	uint8_t pId = crtProc->pId;
	if (irqHandler[IRQn] == 0) {
		irqHandler[IRQn] = pId;
		NVIC_EnableIRQ(IRQn);
		kPrintf("Connected IRQ %d to process %d.\n", IRQn, pId);
	}
}

void irqDisconnect(uint8_t IRQn) {
	uint8_t pId = crtProc->pId;
	if (irqHandler[IRQn] == pId) {
		irqHandler[IRQn] = 0;
		NVIC_DisableIRQ(IRQn);
		kPrintf("Disconnected IRQ %d.\n", IRQn);
	}
}

/* Signal that the IRQ has been dealt with. */
void irqAcknowledge(uint8_t IRQn) {
	NVIC_ClearPendingIRQ(IRQn);
	NVIC_EnableIRQ(IRQn);
}

/* Forwards a message from the hardware to the registered handler of interrupt @IRQn. */
void toIRQHandler(uint8_t IRQn, Message msg) {
 	if (IRQn < NUM_IRQS) {
		uint8_t idHandler = irqHandler[IRQn];
		if (idHandler != 0) {
			NVIC_DisableIRQ(IRQn);
			Process *pHandler = &pTable[idHandler];
			/* Send an interrupt message to the handler. */
			if (pHandler->pState == RECEIVING &&
			   (pHandler->pAccept == ACCEPT_ANY || pHandler->pAccept == ACCEPT_HW)) {
				/* The handler is waiting for a message from the hardware. */
				*(pHandler->pMsg) = msg;
				rescheduleProc(pHandler);
				requestContextSwitch();
			} else {
				/* Mark the interrupt as pending. */
				pHandler->pPending |= (1 << IRQn);
			}
		} else {
			kPrintf("Bad IRQn %d!\n", IRQn);
		}
	}
}
