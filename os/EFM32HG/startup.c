#include "em_msc.h"
#include "em_cmu.h"
#include "em_usb.h"
#include "em_lcd.h"
#include "em_chip.h"
#include "em_core.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "em_device.h"

#include "../drivers/usb/inc/cdc.h"
#include "../drivers/temp/inc/temp.h"

#define LED_PORT  gpioPortF
#define PB_PORT   gpioPortC
#define LED_0_PIN 4
#define LED_1_PIN 5
#define PB_0_PIN  9
#define PB_1_PIN  10

void initGPIO(void) {
	/* Configure PB0 and PB1 as input with glitch filter enabled. */
	GPIO_PinModeSet(PB_PORT, PB_0_PIN, gpioModeInputPullFilter, 1);
	GPIO_PinModeSet(PB_PORT, PB_1_PIN, gpioModeInputPullFilter, 1);

	/* Configure LED0 and LED1 as output. */
	GPIO_PinModeSet(LED_PORT, LED_0_PIN, gpioModePushPull, 0);
	GPIO_PinModeSet(LED_PORT, LED_1_PIN, gpioModePushPull, 0);

	/* Set enable and Tx pin high. */
	GPIO_PinModeSet(gpioPortA, 9, gpioModePushPull, 1);
	GPIO_PinModeSet(gpioPortF, 2, gpioModePushPull, 1);

	/* Enable falling-edge interrupts for PB pins. */
	GPIO_ExtIntConfig(PB_PORT, PB_0_PIN, PB_0_PIN, 0, 1, true);
	GPIO_ExtIntConfig(PB_PORT, PB_1_PIN, PB_1_PIN, 0, 1, true);
}

void initUSART(void) {
	/* Set up interface. */
	USART_InitAsync_TypeDef init = USART_INITASYNC_DEFAULT;
	USART_InitAsync(USART1, &init);
	USART1->ROUTE |= USART_ROUTE_TXPEN | USART_ROUTE_LOCATION_LOC4;
}

void initSysTick(void) {
	/* Generate a sysTick interrupt every 1ms. */
	uint32_t ticks = CMU_ClockFreqGet(cmuClock_CORE) / 1000;
	SysTick_Config(ticks);
}

void boardInit() {
	CHIP_Init();

	USBD_Init(&usbInitStruct);

	/* Enable GPIO, USART. */
	CMU_ClockEnable(cmuClock_GPIO, true);
	CMU_ClockEnable(cmuClock_USART1, true);
	CMU_ClockEnable(cmuClock_ADC0, true);

	/* Enable dHFXO and high frequency peripherals */
	CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
	CMU_ClockEnable(cmuClock_HFPER, true);

	adcInit();

	initGPIO();
	initUSART();
	initSysTick();

	MSC_Init();
}

bool useLoaderMode(void) { return !GPIO_PinInGet(PB_PORT, PB_0_PIN); }
