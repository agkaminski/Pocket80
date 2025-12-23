/* Pocket80 Z80 CTC driver
 * A.K. 2025
 */

#ifndef DRIVER_CTC_H_
#define DRIVER_CTC_H_

#define CTC_INTERRUPT_EN  (1 << 7)
#define CTC_PRESCALE_256  (1 << 5)
#define CTC_PRESCALE_16   0
#define CTC_RELOAD        (1 << 2)
#define CTC_RESET         (1 << 1)
#define CTC_CONTROL       (1 << 0)

#include <stdint.h>

/* Preload value is on MSB byte of ctrl */
void ctc_control(uint8_t channel, uint16_t ctrl);

void ctc_init(uint8_t vector);

#endif
