/* Pocket80 Z80 IRQ enable/disable
 * A.K. 2025
 */

#ifndef DRIVER_INTERRUPT_H_
#define DRIVER_INTERRUPT_H_

#define DI() __asm di __endasm
#define EI() __asm ei __endasm

#endif
