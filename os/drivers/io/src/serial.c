#include "../inc/serial.h"

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>

#include "em_cmu.h"
#include "em_usart.h"

#include "../../../kernel/inc/os_exceptions.h"

char serialOutBuffer[SERIAL_BUF_SIZE];

void serialOut(char *message) {
    for (uint32_t i = 0; i < strlen(message); i++ ) {
        USART_Tx(USART1, message[i]);
    }
}

void kPrintf(char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	/* Pass the va_list to sprintf. */
	vsprintf(serialOutBuffer, fmt, args);
	serialOut(serialOutBuffer);
}

