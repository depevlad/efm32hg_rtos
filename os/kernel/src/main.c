#include "../inc/os_mem.h"

int main(void) {
	boardInit();
	boot(useLoaderMode());
}
