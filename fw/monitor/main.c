/* Pocket80 monitor main
 * A.K. 2025
 */

#include <string.h>

#include "driver/dl1414.h"
#include "driver/ctc.h"
#include "driver/sio.h"

static void beep(void)
{
	ctc_control(2, CTC_PRESCALE_16 | CTC_RELOAD | CTC_CONTROL | ((144 / 2) << 8));
	for (volatile int i = 0; i < 1000; ++i);
	ctc_control(2, CTC_RESET | CTC_CONTROL);
}

int main(void)
{
	memcpy(dl1414_buff, "[ POCKET80 ]", 12);
	dl1414_update();

	/* Buzzer test */
	beep();

	/* Baud generator */
	ctc_control(1, CTC_PRESCALE_16 | CTC_RELOAD | CTC_CONTROL | (1 << 8));

	sio_init();

	for (size_t i = 0; i < 12; ++i) {
		sio_write(0, dl1414_buff[i]);
	}

	sio_write(0, '\r');
	sio_write(0, '\n');

	for (;;) {
		uint8_t data = sio_read(0);
		sio_write(0, data);

		beep();

		memmove(dl1414_buff + 1, dl1414_buff, 11);
		dl1414_buff[0] = data;
		dl1414_update();
	}

	return 0;
}
