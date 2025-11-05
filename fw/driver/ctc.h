/* Pocket80 Z80 CTC driver
 * A.K. 2025
 */

#ifndef DRIVER_CTC_H_
#define DRIVER_CTC_H_

#define CTC_INTERRUPT_EN  (1 << 7)
#define CTC_COUNTER       (1 << 6)
#define CTC_TIMER         0
#define CTC_PRESCALE_256  (1 << 5)
#define CTC_PRESCALE_16   0

#include <stdint.h>

struct ctc_control {
	uint8_t interrupt;
	enum { ctc_timer, ctc_counter } mode;
	enum { ctc_prescaler_16, ctc_prescaler_256 } prescaler;
	enum { ctc_edge_falling, ctc_edge_rising } edge;
	enum { ctc_trigger_auto, ctc_trigger_clk } trigger;
	struct {
		uint8_t constant;
		uint8_t present;
	} time;
	uint8_t reset;
};

void ctc_control(uint8_t channel, struct ctc_control *control);

void ctc_init(uint8_t vector);

#endif
