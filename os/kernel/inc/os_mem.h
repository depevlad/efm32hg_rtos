#ifndef KERNEL_INC_OS_MEM_H_
#define KERNEL_INC_OS_MEM_H_

#include "stdbool.h"
#include "stdint.h"

#include "os_proc.h"

extern uint8_t crtId;
extern void *__break;

bool writeFlashPage(uint32_t* dst, uint32_t* src);
void startProgram(uint8_t *pStart);

/* Flash. */
#define USER_START_PAGE 56
#define USER_END_PAGE   64
#define PAGE_SIZE     1024

#define MAGIC_NR 0x01234567

#define KERNEL_STACK_TOP   (0x20001400)

#endif /* KERNEL_INC_OS_MEM_H_ */
