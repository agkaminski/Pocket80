/* Pocket80 Z80 IRQ enable/disable
 * A.K. 2025
 */

#include <stdint.h>

static uint8_t ready = 0;

void interrupt_ei(void)
{
	if (ready) {
		__asm ei; __endasm;
	}
}

void interrupt_enable(void)
{
	ready = 1;
}
