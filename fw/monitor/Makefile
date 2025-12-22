TARGET = monitor

CC = $(shell if `which sdcc-sdcc > /dev/null 2>&1`; then echo "sdcc-sdcc"; else echo "sdcc"; fi)
AS = $(shell if `which sdcc-sdasz80 > /dev/null 2>&1`; then echo "sdcc-sdasz80"; else echo "sdasz80"; fi)

CPU = z80


CODE = 0x8200
DATA = 0xC000
GHASH := "$(shell git describe --always --dirty)"
DATE := "$(shell date +"%X %x")"
CFLAGS = -m${CPU} -DVERSION='${GHASH}' -DDATE='${DATE}' --opt-code-size --max-allocs-per-node 10000
LDFLAGS = --code-loc ${CODE} --data-loc ${DATA} --no-std-crt0

SRC = crt0.s
SRC += main.c

OBJ = $(SRC:.c=.rel)
OBJ := $(OBJ:.s=.rel)

LIBPATH := ../lib/
INCPATH := ../include/

LIB := driver.lib

CFLAGS += -L${LIBPATH} -I${INCPATH}

.PHONY: clean install

all: ${TARGET}.bin

${TARGET}.bin: ${TARGET}.hex
	@objcopy -Iihex -Obinary ${TARGET}.hex ${TARGET}.bin
	du -b ${TARGET}.bin

${TARGET}.hex: ${OBJ} ${LIB}
	$(CC) -o ${TARGET}.hex ${CFLAGS} ${LDFLAGS} ${OBJ} ${LIB}

%.rel: %.c headers
	$(CC) ${CFLAGS} -o "$@" -c "$<"

%.rel: %.s
	$(AS) -l -o -s "$<"

driver.lib:
	@make --no-print-directory -C ../driver all install

headers:
	@make --no-print-directory -C ../driver install-headers

install: ${TARGET}.bin
	minipro -pAT28C256@PLCC32 -sw ${TARGET}.bin

clean:
	@rm -f *.bin *.lk *.map *.mem *.lst *.rel *.rst *.sym *.asm *.ihx *.noi *.hex
	@make --no-print-directory -C ../driver clean
