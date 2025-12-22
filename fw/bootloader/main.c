/* Pocket80 monitor main
 * A.K. 2025
 */

#include <string.h>

#include "driver/dl1414.h"

int main(void)
{
	memcpy(dl1414_buff, " Bootloader ", 12);
	dl1414_update();

	return 0;
}
