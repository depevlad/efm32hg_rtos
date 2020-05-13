#include "../../kernel/inc/os_msg.h"
#include "../../kernel/inc/os_syscall.h"

#include "../../drivers/temp/inc/temp.h"

Message msg;

void tempSensor(void) {
	while (1) {
		sysReceive(ACCEPT_ANY, &msg);
		if (msg.mType == 0) {
			/* Read temperature and send it back. */
			msg.mDatum.mInt = measureTemperature();
			sysSend(msg.mSender, &msg);
		}
	}
}
