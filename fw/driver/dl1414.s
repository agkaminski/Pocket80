; Pocket80 DL1414 driver
; dl1414.s
; Copyright: Aleksander Kaminski 2025
; See LICENSE.md

.module dl1414

.z80

.area _BSS

.globl _dl1414_buff
_dl1414_buff: .ds 12

.area _CODE

.globl _dl1414_update
_dl1414_update:
		ld hl, #_dl1414_buff
		ld bc, #0x04a3
		call _dl1414_out
		ld bc, #0x04c3
		call _dl1414_out
		ld bc, #0x04e3
_dl1414_out:
		outi
		jr z, _dl1414_out_e
		dec c
		jr _dl1414_out
_dl1414_out_e:
		ret
