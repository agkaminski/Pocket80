/* Pocket80 Z80 SIO driver
 * A.K. 2025
 */

#include <stdint.h>

#include "interrupt.h"

__sfr __at(0x20) DATA_A;
__sfr __at(0x21) DATA_B;

extern void sio_reg_write(uint8_t channel, uint8_t reg, uint8_t val);

extern uint8_t sio_reg_read(uint8_t channel, uint8_t reg);

void sio_write(uint8_t channel, uint8_t byte)
{
	uint8_t rr0;

	do {
		rr0 = sio_reg_read(channel, 0);
	} while (!(rr0 & (1 << 2)));

	if (channel) {
		DATA_B = byte;
	}
	else {
		DATA_A = byte;
	}
}

uint8_t sio_read(uint8_t channel)
{
	uint8_t rr0;

	do {
		rr0 = sio_reg_read(channel, 0);
	} while (!(rr0 & (1 << 0)));

	if (channel) {
		return DATA_B;
	}
	else {
		return DATA_A;
	}
}

void sio_init(void)
{
	/* Channel reset */
	sio_reg_write(0, 0, 0x18);
	sio_reg_write(0, 0, 0x10);

	/* 1 stop bit, x16 clock, no parity */
	sio_reg_write(0, 4, 0x44);

	/* No interrupts */
	sio_reg_write(0, 1, 0);

	/* Interrupt vector */
	// sio_reg_write(0, 2, 0x00);

	/* Receiver enable, 8 bits, no BS */
	sio_reg_write(0, 3, 0xc1);

	/* Transmitter enable, 8 bits, no BS */
	sio_reg_write(0, 5, 0x68);
}
