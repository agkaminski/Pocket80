; Pocket80 monitor
; crt0.s
; Copyright: Aleksander Kaminski 2025
; See LICENSE.md

; Watch out! Only 0x200 bytes for this code!

.module ctr0

.globl _main

.z80

.area _HEADER (ABS)
.org 0x0000

reset:
			; Setup inital stack
			ld sp, #0x0000

			; Init .bss and .data
			call gsinit

			; Setup IVT
			ld a, #0x01 ; 0x0100 >> 8
			ld i, a

			call _main

			di
			halt

.org 0x0100
ivt:
; TODO

.macro SAVE
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
.endm

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

