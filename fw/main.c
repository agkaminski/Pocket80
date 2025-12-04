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

#if 1
	/* Baud generator */
	struct ctc_control ctc = {
		.interrupt = 0,
		.mode = ctc_timer,
		.prescaler = ctc_prescaler_16,
		.edge = ctc_edge_falling,
		.trigger = ctc_trigger_auto,
		.time = { .present = 1, .constant = 1 },
		.reset = 0
	};
	ctc_control(1, &ctc);
#endif

	sio_init();
	for (unsigned int i = 0;; ++i) {
		for (volatile uint16_t j = 0; j < 0x7fff; ++j) {
			__asm nop __endasm;
		}
		sio_tx('H');
		sio_tx('e');
		sio_tx('l');
		sio_tx('l');
		sio_tx('o');
		sio_tx(' ');
		sio_tx('W');
		sio_tx('o');
		sio_tx('r');
		sio_tx('l');
		sio_tx('d');
		sio_tx('!');
		sio_tx('\r');
		sio_tx('\n');

		dl1414_buff[0] = '0' + (i % 10);
		dl1414_update();
	}

	return 0;
}
