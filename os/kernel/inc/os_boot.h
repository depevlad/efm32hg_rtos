#ifndef KERNEL_INC_OS_BOOT_H_
#define KERNEL_INC_OS_BOOT_H_

typedef void (*fPtr) (void);

typedef struct {
	char    *tName;   /* Name of the system task. */
	fPtr     tBody;   /* Pointer to the body of the system task. */
	uint16_t tSize;   /* System task stack space, in bytes. */
} Task;


#endif /* KERNEL_INC_OS_BOOT_H_ */
