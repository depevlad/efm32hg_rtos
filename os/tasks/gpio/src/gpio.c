/*
 * gpio.c
 *
 *  Created on: Apr 23, 2020
 *      Author: depevlad
 */
#include "em_gpio.h"

#include "../../../kernel/inc/os_msg.h"
#include "../../../kernel/inc/os_syscall.h"

#define PB_0_PIN 9
#define PB_1_PIN 10

#define IS_SET(x, pos) ((x & (1 << pos)) != 0)

typedef void (*pbCallback) (void);

pbCallback pbCallbacks[2];
Message msg;

void gpioHandler(void) {
	sysConnectIrq(GPIO_EVEN_IRQn);
	sysConnectIrq(GPIO_ODD_IRQn);
	while (1) {
		/* Get an interrupt message and acknowledge it. */
		sysReceive(ACCEPT_ANY, &msg);

		/* Either dispatch a callback... */
		if (msg.mType == M_HARDWARE) {
			sysAcknowledgeIrq(msg.mSender);
			uint32_t iFlags = msg.mDatum.mInt;
			if (IS_SET(iFlags, PB_0_PIN)) {
				if (pbCallbacks[0] != NULL) {
					pbCallbacks[0]();
				}
			}
			if (IS_SET(iFlags, PB_1_PIN)) {
				if (pbCallbacks[1] != NULL) {
					pbCallbacks[1]();
				}
			}
		}
		/* Or register one. */
		else {
			uint8_t pbID = msg.mType;
			pbCallback f = msg.mDatum.mPtr;
			pbCallbacks[pbID] = f;
		}
	}
}
