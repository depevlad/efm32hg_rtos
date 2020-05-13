#include "../inc/os_mem.h"
#include "../inc/os_proc.h"
#include "../inc/os_boot.h"

#include "../../drivers/io/inc/serial.h"
#include "../../drivers/temp/inc/temp.h"

void idle(void);
void loader(void);
void ledHandler(void);
void gpioHandler(void);
void tempSensor(void);

#define makeTask(_tName, _tBody, _tSize) {    \
		.tName = _tName,                      \
		.tBody = _tBody,                      \
		.tSize = _tSize                       \
}                                             \

Task sysTasks[] = {
	makeTask("idle", &idle, 128),
	makeTask("loader", &loader, 512),
	makeTask("gpio", &gpioHandler, 128),
	makeTask("leds", &ledHandler, 128),
	makeTask("tempsens", &tempSensor, 128)
};

void startTask(uint8_t tId) {
	startProc(
		tId,
		sysTasks[tId].tName,
		sysTasks[tId].tBody,
		__break,
		sysTasks[tId].tSize
	);
	__break += sysTasks[tId].tSize;
}

void boot(bool loaderMode) {
	memset(pTable, 0, sizeof(NUM_PROCS * sizeof(Process)));
	__break = KERNEL_STACK_TOP;

	kPrintf("Booting...\n");

	/* Start idle. */
	startTask(0);

	if (loaderMode) {
		/* Only start idle and loader. */
		startTask(1);
		kPrintf("Booted in loader mode.\n");
		startScheduler();
		return;
	}

	uint8_t nSysTasks = sizeof(sysTasks) / sizeof(Task);
	for (uint8_t i = 2; i < nSysTasks; i++) {
		startTask(i);
		kPrintf("Started task %s\n", sysTasks[i].tName);
	}

	crtId = nSysTasks;
	for (uint8_t i = USER_START_PAGE; i < USER_END_PAGE; i++) {
		uint8_t *pStart = i * PAGE_SIZE;
		if (*((uint32_t*)pStart) == MAGIC_NR)
			startProgram(pStart);
	}

	startScheduler();
}
