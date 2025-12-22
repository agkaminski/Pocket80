; Pocket80 SIO low level driver
; sio_low.s
; Copyright: Aleksander Kaminski 2025
; See LICENSE.md

.module sio_low

.z80

.globl _interrupt_ei

.area _CODE

.globl _sio_reg_write
_sio_reg_write: ; void sio_reg_write(uint8_t channel, uint8_t reg, uint8_t val)
		add a, #0x22
		ld c, a
		ld a, l

		pop hl
		pop de
		dec sp
		push hl

		di
		or a, a
		jr z, $1
		out (c), a
$1:		out (c), e
		jp _interrupt_ei


.globl _sio_reg_read
_sio_reg_read: ; uint8_t sio_reg_read(uint8_t channel, uint8_t reg)
		add a, #0x22
		ld c, a
		di
		out (c), l
		in a, (c)
		push af
		call _interrupt_ei
		pop af
		ret
