/* Pocket80 monitor main
 * A.K. 2025
 */

#include <string.h>
#include <ctype.h>

#include "driver/dl1414.h"
#include "driver/ctc.h"
#include "driver/sio.h"
#include "driver/interrupt.h"

static void beep(void)
{
	ctc_control(2, CTC_PRESCALE_16 | CTC_RELOAD | CTC_CONTROL | ((144 / 2) << 8));
	for (volatile int i = 0; i < 1000; ++i);
	ctc_control(2, CTC_RESET | CTC_CONTROL);
}

void dlprint(const char *str)
{
	memset(dl1414_buff, ' ', sizeof(dl1414_buff));
	strncpy(dl1414_buff, str, sizeof(dl1414_buff));
	for (size_t i = 0; i < sizeof(dl1414_buff); ++i) {
		dl1414_buff[i] = toupper(dl1414_buff[i]);
	}
	dl1414_update();
}

int main(void)
{
	static const char welcome_msg[] = "[ POCKET80 ]";

	dlprint(welcome_msg);

	/* Baud generator */
	_ctc_control(1, CTC_PRESCALE_16 | CTC_RELOAD | CTC_CONTROL | (1 << 8));

	sio_init(0x00);

	EI();

	beep();

	for (;;) {
		sio_write(0, welcome_msg, sizeof(welcome_msg) - 1);
		sio_write(0, "\r\n", 2);
	}

	for (;;) {
		uint8_t data;

		sio_read(0, &data, 1);
		sio_write(0, &data, 1);

		beep();

		memmove(dl1414_buff + 1, dl1414_buff, 11);
		dl1414_buff[0] = data;
		dl1414_update();
	}

	return 0;
}
