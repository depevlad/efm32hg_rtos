/*
 * leds.c
 *
 *  Created on: Apr 24, 2020
 *      Author: depevlad
 */

#include "assert.h"

#include "em_gpio.h"

#include "../../../kernel/inc/os_msg.h"
#include "../../../kernel/inc/os_syscall.h"

#define NUM_LEDS 2

#define LEDS_PORT gpioPortF
#define LED_0_PIN 4
#define LED_1_PIN 5

Message msg;

void ledToggle(uint8_t ledId) {
	uint8_t pinId = LED_0_PIN;
	if (ledId == 1) pinId = LED_1_PIN;
	GPIO_PinOutToggle(LEDS_PORT, pinId);
}

void ledHandler(void) {
	/* Initialise the pins. */
	GPIO_PinModeSet(LEDS_PORT, LED_0_PIN, gpioModePushPull, 0);
	GPIO_PinModeSet(LEDS_PORT, LED_1_PIN, gpioModePushPull, 0);
	while (1) {
		sysReceive(ACCEPT_ANY, &msg);
		/* Toggle a LED. */
		if (msg.mType == 0) {
			uint8_t ledId = msg.mDatum.mInt;
			assert(0 <= ledId && ledId < NUM_LEDS);
			ledToggle(ledId);
		}
	}
}
