/* Pocket80 monitor main
 * A.K. 2025
 */

#include <string.h>

#include "driver/dl1414.h"
#include "driver/ctc.h"

int main(void)
{
	memcpy(dl1414_buff, "[ POCKET80 ]", 12);
	dl1414_update();

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

	return 0;
}
