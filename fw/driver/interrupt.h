/* Pocket80 Z80 IRQ enable/disable
 * A.K. 2025
 */

#ifndef DRIVER_INTERRUPT_H_
#define DRIVER_INTERRUPT_H_

void interrupt_ei(void);

#define DI() __asm di __endasm
#define EI() interrupt_ei()

#endif
