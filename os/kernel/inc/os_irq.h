/*
 * os_irq.h
 *
 *  Created on: Apr 16, 2020
 *      Author: depevlad
 */

#ifndef KERNEL_INC_OS_IRQ_H_
#define KERNEL_INC_OS_IRQ_H_

#include "stdint.h"

#include "../inc/os_msg.h"

#define NUM_IRQS 32

extern uint8_t irqHandler[];

void irqConnect(uint8_t IRQn);
void irqDisconnect(uint8_t IRQn);
void irqAcknowledge(uint8_t IRQn);

void toIRQHandler(uint8_t IRQn, Message msg);

#endif /* KERNEL_INC_OS_IRQ_H_ */
