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
			call bss_init
			call data_init

			; Setup IVT
			ld a, #0x01 ; 0x0100 >> 8
			ld i, a

			; TODO

			jp _main

.org 0x0080

.globl _dl1414_buff
.globl dl1414_update

; This function perform rev A bug workaround.
; Write timining for DL1414 is wrong - only
; decoded address is used to generate the
; write strobe, without /WR gating.
; This causes DL1414 to write the data not
; to the desired register, but to the
; register pointed by the address bus
; **after** the write cycle. So it will use
; the 2 LSB of the address of the instruction
; after out. We can circumvent this issue by
; aligning the code in such way, the the
; address of the next insctruction actually
; matches the desired register. Interrupts
; has to be disabled for this - unfortunately
; timed interrupt would mess everything up
; anyway.
; This code will work on rev B too - it will
; be just kinda inefficient.
dl1414_update:
			ld hl, #_dl1414_buff ; 0x83

			di                   ; 0x84
			ld a, (hl)           ; 0x85
			out (0xA3), a        ; 0x87
			ei                   ; 0X88
			inc hl               ; 0x89
			ld a, (hl)           ; 0x8A
			di                   ; 0X8B
			nop                  ; 0X8C
			out (0xA2), a        ; 0x8E
			ei                   ; 0x8F
			inc hl               ; 0x90
			ld a, (hl)           ; 0x91
			di                   ; 0x92
			nop                  ; 0x93
			out (0xA1), a        ; 0x95
			ei                   ; 0x96
			inc hl               ; 0x97
			ld a, (hl)           ; 0x98
			di                   ; 0x99
			nop                  ; 0x9A
			out (0xA0), a        ; 0x9C
			ei                   ; 0x9D
			inc hl               ; 0x9E

			di                   ; 0x9F
			nop                  ; 0xA0

			ld a, (hl)           ; 0xA1
			out (0xC3), a        ; 0xA3
			ei                   ; 0xA4
			inc hl               ; 0xA5
			ld a, (hl)           ; 0xA6
			di                   ; 0xA7
			nop                  ; 0xA8
			out (0xC2), a        ; 0xAA
			ei                   ; 0xAB
			inc hl               ; 0xAC
			ld a, (hl)           ; 0xAD
			di                   ; 0xAE
			nop                  ; 0xAF
			out (0xC1), a        ; 0xB1
			ei                   ; 0xB2
			inc hl               ; 0xB3
			ld a, (hl)           ; 0xB4
			di                   ; 0xB5
			nop                  ; 0xB6
			out (0xC0), a        ; 0xB8
			ei                   ; 0xB9
			inc hl               ; 0xBA

			di                   ; 0xBB
			nop                  ; 0xBC

			ld a, (hl)           ; 0xBD
			out (0xE3), a        ; 0xBF
			ei                   ; 0xC0
			inc hl               ; 0xC1
			ld a, (hl)           ; 0xC2
			di                   ; 0xC3
			nop                  ; 0xC4
			out (0xE2), a        ; 0xC6
			ei                   ; 0xC7
			inc hl               ; 0xC8
			ld a, (hl)           ; 0xC9
			di                   ; 0xCA
			nop                  ; 0xCB
			out (0xE1), a        ; 0xCD
			ei                   ; 0xCE
			inc hl               ; 0xCF
			ld a, (hl)           ; 0xD0
			di                   ; 0xD1
			nop                  ; 0xD2
			out (0xE0), a        ; 0xD4
			ei                   ; 0xD5

			ret


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

.area _GSINIT

.globl l__DATA
.globl s__DATA
.globl l__INITIALIZER
.globl s__INITIALIZER
.globl s__INITIALIZED

bss_init:
			ld bc, #l__DATA ; Length to zero-out
			ld a, b         ; Check if zero
			or a, c
			ret z

			ld hl, #s__DATA ; Start of the .bss
			ld (hl), #0     ; Zero-out first byte
			dec bc          ; One byte already done
			ld a, b         ; Check if zero
			or a, c
			ret z

			ld d, h
			ld e, l
			inc de
			ldir            ; Copy first zeroed byte

			ret

data_init:
			ld bc, #l__INITIALIZER
			ld a, b         ; Check if zero
			or a, c
			ret z

			ld	de, #s__INITIALIZED
			ld	hl, #s__INITIALIZER
			ldir

			ret

.area _CODE

.globl __bss_end

__bss_end: ; uint16_t _bss_end(void)
			ld de, #s__DATA
			ld hl, #l__DATA
			add hl, de
			push hl
			pop de
			ret
