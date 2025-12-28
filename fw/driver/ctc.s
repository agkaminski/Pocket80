; Pocket80 Z80 CTC driver
; ctc.s
; Copyright: Aleksander Kaminski 2025
; See LICENSE.md

.module ctc

.z80

.area _CODE

.globl __ctc_control
__ctc_control: ; void _ctc_control(uint8_t channel, uint16_t ctrl)
		ld c, a
		out (c), e
		bit 2, e
		jr z, 1$
		out (c), d
1$:		ret


.globl _ctc_control
_ctc_control: ; void ctc_control(uint8_t channel, uint16_t ctrl)
		di
		call __ctc_control
		ei
		ret


.globl _ctc_init
_ctc_init: ; void ctc_init(uint8_t vector)
		and #0xf8
		out (0x00), a
		ret
