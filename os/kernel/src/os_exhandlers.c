/*
 * os_handlers.c
 *
 *  Created on: Apr 22, 2020
 *      Author: depevlad
 */

#include "em_gpio.h"

#include "../inc/os_irq.h"
#include "../inc/os_msg.h"
#include "../inc/os_proc.h"
#include "../inc/os_exceptions.h"

/* ===== CONTEXT SWITCHING ===== */

void __attribute((naked)) saveContext() {
	__asm__ volatile(
		".syntax unified   \n"
		"mrs r0, psp       \n"
		"subs r0, #16      \n"
		"stm r0!, {r4-r7}  \n"
		"mov r4, r8        \n"
		"mov r5, r9        \n"
		"mov r6, r10       \n"
		"mov r7, r11       \n"
		"subs r0, #32      \n"
		"stm r0!, {r4-r7}  \n"
		"subs r0, #16      \n"
		"bx lr             \n"
	);
}

void __attribute((naked)) restoreContext() {
	__asm__ volatile(
		".syntax unified   \n"
		"ldm r0!, {r4-r7}  \n"
		"mov r8, r4        \n"
		"mov r9, r5        \n"
		"mov r10, r6       \n"
	    "mov r11, r7       \n"
		"ldm r0!, {r4-r7}  \n"
		"msr psp, r0       \n"
		"bx lr             \n"
	);
}

#define __wrap(f)                      \
	__asm__ volatile("push {lr, r0}"); \
	saveContext();                     \
	f();                               \
	restoreContext();                  \
	__asm__ volatile("pop {r0, pc}");  \

/* ===== HANDLERS ===== */

/* Cortex-M internal exceptions. */

void __attribute((naked)) SVC_Handler()    { __wrap(syscallHandler)   }
void __attribute((naked)) PendSV_Handler() { __wrap(cxtSwitchHandler) }
void HardFault_Handler(void)               { kPrintf("HardFault!\n"); }

void SysTick_Handler(void) {
	for (uint8_t i = 0; i < NUM_PROCS; i++) {
		if (pTable[i].pState == SLEEPING) {
			if (pTable[i].pSleepTime == 0) {
				/* Wake up. */
				makeReady(&pTable[i]);
			}
			pTable[i].pSleepTime -= 1;
		}
	}
}

/* Peripheral interrupt handlers. */

#define GPIO_ODD_MASK  0xaaaaaaaa
#define GPIO_EVEN_MASK 0x55555555

void GPIO_EVEN_IRQHandler(void) {
	uint32_t iFlags = GPIO_IntGet() & GPIO_EVEN_MASK;
	GPIO_IntClear(iFlags);
	Message msg = {
		.mSender = GPIO_EVEN_IRQn,
		.mType   = M_HARDWARE,
		.mDatum  = iFlags
	};
	toIRQHandler(GPIO_EVEN_IRQn, msg);
}

void GPIO_ODD_IRQHandler(void) {
	uint32_t iFlags = GPIO_IntGet() & GPIO_ODD_MASK;
	GPIO_IntClear(iFlags);
	Message msg = {
		.mSender = GPIO_ODD_IRQn,
		.mType   = M_HARDWARE,
		.mDatum  = iFlags
	};
	toIRQHandler(GPIO_ODD_IRQn, msg);
}
