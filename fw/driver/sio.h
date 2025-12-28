/* Pocket80 Z80 SIO driver
 * A.K. 2025
 */

#ifndef DRIVER_SIO_H_
#define DRIVER_SIO_H_

#include <stdint.h>
#include <stddef.h>

void sio_write(uint8_t channel, const void *buff, size_t bufflen);

void sio_read(uint8_t channel, void *buff, size_t bufflen);

size_t sio_read_nb(uint8_t channel, void *buff, size_t bufflen);

void sio_init(uint8_t vector);

#endif
