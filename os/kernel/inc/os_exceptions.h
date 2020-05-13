#ifndef KERNEL_INC_OS_EXCEPTIONS_H_
#define KERNEL_INC_OS_EXCEPTIONS_H_

/* Exception frame layout (64 bytes, 16 words):
 *   == Saved by hardware ==
 *   |  15 - PSR
 *   |  14 - PC
 *   |  13 - LR
 *   |  12 - R12
 *   |  11 - R3
 *   |  10 - R2
 *   |  9  - R1
 *   |  8  - R0
 *   == Saved manually ==
 *   |  7  - R7
 *   |  6  - R6
 *   |  5  - R5
 *   |  4  - R4
 *   |  3  - R11
 *   |  2  - R10
 *   |  1  - R9
 *   |  0  - R8   <-- Stack pointer
 */

/* Offsets of saved registers within an exception frame. */

#define SAVED_R0  8
#define SAVED_R1  9
#define SAVED_R2  10
#define SAVED_R3  11
#define SAVED_R12 12
#define SAVED_LR  13
#define SAVED_PC  14
#define SAVED_PSR 15
#define SAVED_R7  7
#define SAVED_R6  6
#define SAVED_R5  5
#define SAVED_R4  4
#define SAVED_R11 3
#define SAVED_R10 2
#define SAVED_R9  1
#define SAVED_R8  0

#endif /* KERNEL_INC_OS_EXCEPTIONS_H_ */
