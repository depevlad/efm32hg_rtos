#include "../lib/inc/os_syscalls.h"

Message msg;

/* ID of GPIO and LED handler processes. */
const int gpioID = 2;
const int ledID  = 3;

void pb0_Callback(void) {
    /* Toggle LED 0. */
    msg.mType = 0;
    msg.mDatum.mInt = 0;
    sysSend(ledID, &msg);
}

void pb1_Callback(void) {
    /* Toggle LED 1. */
    msg.mType = 0;
    msg.mDatum.mInt = 1;
    sysSend(ledID, &msg);
}

int main(void) {   
    /* Register button 0 callback.*/
    msg.mType = 0;
    msg.mDatum.mPtr = &pb0_Callback;
    sysSend(gpioID, &msg);

    /* Register button 1 callback. */
    msg.mType = 1;
    msg.mDatum.mPtr = &pb1_Callback;
    sysSend(gpioID, &msg);

    sysPrint("Process LED: Registered push button callbacks.\n");
    sysExit();
    return 0;
}
