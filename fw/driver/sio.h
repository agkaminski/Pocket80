/* Pocket80 Z80 SIO driver
 * A.K. 2025
 */

#ifndef DRIVER_SIO_H_
#define DRIVER_SIO_H_

#include <stdint.h>

void sio_tx(uint8_t byte);

void sio_init(void);

#endif
