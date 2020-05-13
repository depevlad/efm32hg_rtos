#ifndef KERNEL_INC_OS_MSG_H_
#define KERNEL_INC_OS_MSG_H_

#include "stdint.h"

#define ACCEPT_ANY 127
#define ACCEPT_HW  126

#define M_HARDWARE 0xff

typedef union {
	uint32_t mInt;
	void    *mPtr;
	struct {
		uint8_t mBw;
		uint8_t mBx;
		uint8_t mBy;
		uint8_t mBz;
	};
} mData;

typedef struct {
	uint8_t mType;
	uint8_t mSender;
	mData   mDatum;
} Message;

#include "../inc/os_proc.h"

void sendMessage(uint8_t dst, Message *msg);
void receiveMessage(uint8_t idSrc, Message *msg);

#endif /* KERNEL_INC_OS_MSG_H_ */
