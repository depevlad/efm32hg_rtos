/*
 * os_mem.c
 *
 *  Created on: Apr 23, 2020
 *      Author: depevlad
 */

#include "../inc/os_mem.h"
#include "../inc/os_proc.h"

#include "em_msc.h"
#include "em_core.h"

uint8_t crtId;
void *__break;

/* Layout of a program in flash:
 * 		0-3  : MAGIC NR
 * 		4-7  : ENTRY
 * 		8-23 : NAME
 * 		24-25: RAM SPACE
 * 		26-27: STK SPACE
 */

void startProgram(uint8_t *pStart) {
	uint8_t *addrBody     = pStart +  4;
	uint8_t *addrName     = pStart +  8;
	uint8_t *addrDataSize = pStart + 24;
	uint8_t *addrStkSize  = pStart + 26;

	void *stkStart   = (void*)((uint8_t*)__break + *((uint16_t*)addrDataSize));
	uint16_t stkSize = *((uint16_t*)addrStkSize);

	startProc(
		crtId,
		(char*)addrName,
		(void (*) (void))(*((uint32_t*)addrBody)),
		stkStart,
		stkSize
	);

	kPrintf("Started program %s\n", (char*)addrName);

	__break = (void*)((uint8_t*)stkStart + stkSize);
	crtId++;
}

/* @dst must be aligned to FLASH_PAGE_SIZE. */
bool __attribute((section(".ram"))) writeFlashPage(uint32_t* dst, uint32_t* src) {
    MSC_Status_TypeDef status;
    CORE_DECLARE_IRQ_STATE;
    CORE_ENTER_ATOMIC();
    MSC->LOCK = MSC_UNLOCK_CODE;
    status = MSC_ErasePage(dst);
    if (status == mscReturnOk) {
    	status = MSC_WriteWord(dst, src, FLASH_PAGE_SIZE);
    }
    MSC->LOCK = 0;
    CORE_EXIT_ATOMIC();
    return (status == mscReturnOk);
}

