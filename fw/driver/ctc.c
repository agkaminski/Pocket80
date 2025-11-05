/* Pocket80 Z80 CTC driver
 * A.K. 2025
 */

#include "ctc.h"
#include "interrupt.h"

__sfr __at(0x00) CHAN0;
__sfr __at(0x01) CHAN1;
__sfr __at(0x02) CHAN2;
__sfr __at(0x03) CHAN3;

void ctc_control(uint8_t channel, struct ctc_control *control)
{
	uint8_t word = 1;

	word |= (control.interrupt)                      ? (1 << 7) : 0;
	word |= (control.mode == ctc_counter)            ? (1 << 6) : 0;
	word |= (control.prescaler == ctc_prescaler_256) ? (1 << 5) : 0;
	word |= (control.edge == ctc_edge_rising)        ? (1 << 4) : 0;
	word |= (control.trigger == ctc_trigger_clk)     ? (1 << 3) : 0;
	word |= (control.time.present)                   ? (1 << 2) : 0;
	word |= (control.reset)                          ? (1 << 1) : 0;

	if (control.time.present) {
		DI();
		switch (channel) {
			case 0:
				CHAN0 = word;
				CHAN0 = control.time.constant;
				break;
			case 1:
				CHAN1 = word;
				CHAN1 = control.time.constant;
				break;
			case 2:
				CHAN2 = word;
				CHAN2 = control.time.constant;
				break;
			case 3:
				CHAN3 = word;
				CHAN3 = control.time.constant;
				break;
		}
		EI();
	}
	else {
		switch (channel) {
			case 0:
				CHAN0 = word;
				break;
			case 1:
				CHAN1 = word;
				break;
			case 2:
				CHAN2 = word;
				break;
			case 3:
				CHAN3 = word;
				break;
		}
	}
}

void ctc_init(uint8_t vector)
{
	CHAN0 = vector & 0xF8;
}
