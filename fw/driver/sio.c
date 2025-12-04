/* Pocket80 Z80 SIO driver
 * A.K. 2025
 */

#include <stdint.h>

#include "interrupt.h"

#define SIO_RX_AVAILABLE (1 << 0)
#define SIO_

__sfr __at(0x20) DATA_A;
__sfr __at(0x21) DATA_B;
__sfr __at(0x22) CONTROL_A;
__sfr __at(0x23) CONTROL_B;

static void sio_reg_write(uint8_t channel, uint8_t reg, uint8_t val)
{
	uint8_t wr0 = reg;

	if (reg == 0) {
		wr0 = val & 0xF8;
	}

	// DI();
	if (channel != 0) {
		CONTROL_B = wr0;
		if (reg != 0) {
			CONTROL_B = val;
		}
	}
	else {
		CONTROL_A = wr0;
		if (reg != 0) {
			CONTROL_A = val;
		}
	}
	// EI();
}

static uint8_t sio_reg_read(uint8_t channel, uint8_t reg)
{
	uint8_t wr0 = reg & 0x3, val;

	// DI();
	if (channel != 0) {
		CONTROL_B = wr0;
		val = CONTROL_B;
	}
	else {
		CONTROL_A = wr0;
		val = CONTROL_A;
	}
	// EI();

	return val;
}

void sio_tx(uint8_t byte)
{
	uint8_t rr0;

	do {
		rr0 = sio_reg_read(0, 0);
	} while (!(rr0 & (1 << 2)));

	DATA_A = byte;
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
