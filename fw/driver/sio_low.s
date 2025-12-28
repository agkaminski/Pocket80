; Pocket80 SIO low level driver
; sio_low.s
; Copyright: Aleksander Kaminski 2025
; See LICENSE.md

.module sio_low

.z80

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

		or a, a
		jr z, $1
		out (c), a
$1:		out (c), e
		ret


.globl _sio_reg_read
_sio_reg_read: ; uint8_t sio_reg_read(uint8_t channel, uint8_t reg)
		add a, #0x22
		ld c, a
		out (c), l
		in a, (c)
		ret


.globl _sio_data_write
_sio_data_write: ; void sio_data_write(uint8_t channel, uint8_t data)
		add a, #0x20
		ld c, a
		out (c), l
		ret


.globl _sio_data_read
_sio_data_read: ; uint8_t sio_data_read(uint8_t channel)
		add a, #0x20
		ld c, a
		in a, (c)
		ret
