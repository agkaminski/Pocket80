/* Pocket80 Z80 SIO driver
 * A.K. 2025
 */

#ifndef DRIVER_SIO_H_
#define DRIVER_SIO_H_

#include <stdint.h>

void sio_write(uint8_t channel, uint8_t byte);

uint8_t sio_read(uint8_t channel);

void sio_init(void);

#endif
