#include "../lib/inc/os_syscalls.h"

Message sendMsg;

/* ID of the LED handler process. */
const int ledID = 3;

void main(void) {
    /* Craft a message that toggles LED 1. */
    sendMsg.mType   = 0;
    sendMsg.mDatum.mInt = 0;

    while (1) {
        sysSleep(1000);
        sysSend(ledID, &sendMsg);
    }
}
