#ifndef TASKS_IDLE_INC_IDLE_H_
#define TASKS_IDLE_INC_IDLE_H_

void idle(void) {
	while (1) {
		/* Wait for an event or interrupt. */
		__asm__ volatile ("wfe");
		requestContextSwitch();
	}
}

#endif /* TASKS_IDLE_INC_IDLE_H_ */
