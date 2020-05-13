#define uint8_t  unsigned char
#define uint32_t unsigned

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