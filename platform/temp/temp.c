#include "../lib/inc/os_syscalls.h"

/* Temperature sensor is process 4. */
const int tempSensID = 4;

Message msg;
char t[5];

/* Use our own print function, since stdlib is unavailable. 
 * For two digit integers (since it is a temperature). */
void printInt(int x) {
	t[0] = '0';
	t[1] = '0';
	while (x >= 10) {
		t[0]++;
		x -= 10;
	}
	t[1] += x;
	t[2] = '\n';
	t[3] = 0;
	sysPrint(t);
}

int main() {
	while (1) {
		/* Send a request to read temperature. */
		msg.mType = 0;
		sysSend(tempSensID, &msg);

		/* Receive temperature reading and display it. */
		sysReceive(tempSensID, &msg);

		sysPrint("Temperature: ");
		printInt(msg.mDatum.mInt);

		/* Sleep for five seconds. */
		sysSleep(5000);

	}
	return 0;
}