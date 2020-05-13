#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>

#include "em_core.h"

#include "../../../drivers/usb/inc/cdc.h"
#include "../../../drivers/io/inc/serial.h"

#include "../../../kernel/inc/os_mem.h"
#include "../../../kernel/inc/os_msg.h"
#include "../../../kernel/inc/os_proc.h"
#include "../../../kernel/inc/os_syscall.h"

uint8_t mscBuffer[FLASH_PAGE_SIZE];

void loadBinary(uint8_t pageId) {
	pageId += USER_START_PAGE;
	kPrintf("%d\n", pageId);
	assert(pageId <= USER_END_PAGE);

	uint32_t *addr = (uint32_t*)(pageId * FLASH_PAGE_SIZE);
	kPrintf("Loading program @ %p", addr);
	gloXferred = 0;

	memset(mscBuffer, 0, sizeof(mscBuffer));

	/* Copy header. */
	memcpy(mscBuffer, usbRxBuffer, 28);

	uint32_t buffPos = 32;
	while (1) {
		if (gloXferred > 0) {
			if (*((uint32_t*)usbRxBuffer) == MAGIC_NR) {
				/* This marks the end of the binary. */
				if (buffPos > 0) {
					writeFlashPage(addr, (uint32_t*)mscBuffer);
				}
				return;
			}
			else {
				for (uint8_t i = 0; i < gloXferred; i++) {
					kPrintf("%02X", usbRxBuffer[i]);
					/* Add to buffer. */
					mscBuffer[buffPos] = usbRxBuffer[i];
					buffPos += 1;
					if (buffPos == FLASH_PAGE_SIZE) {
						writeFlashPage(addr, (uint32_t*)mscBuffer);
						addr += (FLASH_PAGE_SIZE / 4);
						buffPos = 0;
					}
				}
				kPrintf("\n");
				gloXferred = 0;
			}
		}
	}
}

void loader() {
	kPrintf("Loading...\n");
	while (1) {
		if (gloXferred > 0) {
			uint32_t pattern = *((uint32_t*)(usbRxBuffer));
			if (pattern == MAGIC_NR) {
				/* Start of a binary. The current frame contains:
				 *   0-3  : MAGIC NR
				 *   4-7  : ENTRY
				 *   8-23 : NAME
				 *   24-25: RAM SPACE
				 *   26-27: STACK SPACE
				 *   28-28: PAGE ID
				 */
				uint8_t pageId = usbRxBuffer[28];
				loadBinary(pageId);
			} else {
				kPrintf("Unrecognized pattern %d!\n", pattern);
			}
			gloXferred = 0;
	    }
	}
}
