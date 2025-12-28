; Pocket80 monitor
; crt0.s
; Copyright: Aleksander Kaminski 2025
; See LICENSE.md

; Watch out! Only 0x200 bytes for this code!

.module ctr0

.globl _main

.z80

.area _HEADER (ABS)

.globl s__CODE

.org 0x0000

reset:
			ld sp, #0x0000

			; Init .bss and .data
			call gsinit

			; Setup interrupts
			im 2
			ld a, #0x01 ; 0x0100 >> 8
			ld i, a

			call _main

.globl _sio_irq_tx_empty
.globl _sio_irq_rx_avail
.globl _sio_irq_special

.macro SAVE
			di
			ex af, af'
			exx
			push ix
			push iy
.endm

.macro RESTORE
			pop iy
			pop ix
			exx
			ex af, af'
			ei
			reti
.endm

__sio_irq_a_tx_empty:
			SAVE
			ld a, #0
			call _sio_irq_tx_empty
			RESTORE

__sio_irq_a_rx_avail:
			SAVE
			ld a, #0
			call _sio_irq_rx_avail
			RESTORE

__sio_irq_a_special:
			SAVE
			ld a, #0
			call _sio_irq_special
			RESTORE

__sio_irq_b_tx_empty:
			SAVE
			ld a, #1
			call _sio_irq_tx_empty
			RESTORE

__sio_irq_b_rx_avail:
			SAVE
			ld a, #1
			call _sio_irq_rx_avail
			RESTORE

__sio_irq_b_special:
			SAVE
			ld a, #1
			call _sio_irq_special
			RESTORE

irq_bad:
			di
			halt
			jr irq_bad

.org 0x0100

ivt:

; SIO

; Channel B
.word __sio_irq_b_tx_empty
.word irq_bad
.word __sio_irq_b_rx_avail
.word __sio_irq_b_special

; Channel A
.word __sio_irq_a_tx_empty
.word irq_bad
.word __sio_irq_a_rx_avail
.word __sio_irq_a_special

.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad

.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad

.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad

.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad

.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad

.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad

.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad

.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad

.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad

.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad

.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad

.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad

.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad

.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad

.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad
.word irq_bad

.area _HOME
.area _CODE
.area _INITIALIZER
.area _GSINIT
.area _GSFINAL

.area _DATA
.area _INITIALIZED
.area _BSEG
.area _BSS
.area _HEAP

.globl l__DATA
.globl s__DATA
.globl l__INITIALIZER
.globl s__INITIALIZER
.globl s__INITIALIZED

.area   _GSINIT
gsinit::
			; Default-initialized global variables.
			ld      bc, #l__DATA
			ld      a, b
			or      a, c
			jr      Z, zeroed_data
			ld      hl, #s__DATA
			ld      (hl), #0x00
			dec     bc
			ld      a, b
			or      a, c
			jr      Z, zeroed_data
			ld      e, l
			ld      d, h
			inc     de
			ldir
zeroed_data:
			; Explicitly initialized global variables.
			ld	bc, #l__INITIALIZER
			ld	a, b
			or	a, c
			jr	Z, gsinit_next
			ld	de, #s__INITIALIZED
			ld	hl, #s__INITIALIZER
			ldir

gsinit_next:

.area   _GSFINAL
			ret

