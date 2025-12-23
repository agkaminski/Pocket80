; Pocket80 Z80 CTC driver
; ctc.s
; Copyright: Aleksander Kaminski 2025
; See LICENSE.md

.module ctc

.z80

.globl _interrupt_ei

.area _CODE

.globl _ctc_control
_ctc_control: ; void ctc_control(uint8_t channel, uint16_t ctrl)
		ld c, a
		di
		out (c), e
		bit 2, e
		jr z, 1$
		out (c), d
1$:		jp _interrupt_ei

.globl _ctc_init
_ctc_init: ; void ctc_init(uint8_t vector)
		and #0xf8
		out (0), a
		ret
