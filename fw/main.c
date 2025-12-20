/* Pocket80 monitor main
 * A.K. 2025
 */

#include <string.h>

#include "driver/dl1414.h"
#include "driver/ctc.h"
#include "driver/sio.h"

int main(void)
{
	memcpy(dl1414_buff, "[ POCKET80 ]", 12);
	dl1414_update();
#if 0
	/* Buzzer test */
	struct ctc_control ctc = {
		.interrupt = 0,
		.mode = ctc_timer,
		.prescaler = ctc_prescaler_16,
		.edge = ctc_edge_falling,
		.trigger = ctc_trigger_auto,
		.time = { .present = 1, .constant = 114/2 },
		.reset = 0
	};
	ctc_control(2, &ctc);
#endif

	/* Baud generator */
	static const struct ctc_control ctc_baud = {
		.interrupt = 0,
		.mode = ctc_timer,
		.prescaler = ctc_prescaler_16,
		.edge = ctc_edge_falling,
		.trigger = ctc_trigger_auto,
		.time = { .present = 1, .constant = 1 },
		.reset = 0
	};
	ctc_control(1, &ctc_baud);

	sio_init();

	for (size_t i = 0; i < 12; ++i) {
		sio_write(0, dl1414_buff[i]);
	}

	sio_write(0, '\r');
	sio_write(0, '\n');

	for (;;) {
		uint8_t data = sio_read(0);
		sio_write(0, data);

		static const struct ctc_control ctc_beep_start = {
			.interrupt = 0,
			.mode = ctc_timer,
			.prescaler = ctc_prescaler_16,
			.edge = ctc_edge_falling,
			.trigger = ctc_trigger_auto,
			.time = { .present = 1, .constant = 114/2 },
			.reset = 0
		};
		static const struct ctc_control ctc_beep_stop = {
			.interrupt = 0,
			.mode = ctc_timer,
			.prescaler = ctc_prescaler_16,
			.edge = ctc_edge_falling,
			.trigger = ctc_trigger_auto,
			.time = { .present = 0, .constant = 0 },
			.reset = 1
		};

		ctc_control(2, &ctc_beep_start);
		for (volatile int i = 0; i < 1000; ++i);
		ctc_control(2, &ctc_beep_stop);

		memmove(dl1414_buff + 1, dl1414_buff, 11);
		dl1414_buff[0] = data;
		dl1414_update();
	}

	return 0;
}
