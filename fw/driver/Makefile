TARGET = driver

.PHONY: clean install

CC = $(shell if `which sdcc-sdcc > /dev/null 2>&1`; then echo "sdcc-sdcc"; else echo "sdcc"; fi)
AS = $(shell if `which sdcc-sdasz80 > /dev/null 2>&1`; then echo "sdcc-sdasz80"; else echo "sdasz80"; fi)
AR = $(shell if `which sdcc-sdar > /dev/null 2>&1`; then echo "sdcc-sdar"; else echo "sdar"; fi)

CPU = z80
CFLAGS = -m${CPU} -DVERSION='${GHASH}' --opt-code-size --max-allocs-per-node 10000

SRC = interrupt.c ctc.s sio.c sio_low.s dl1414.s

OBJ = $(SRC:.c=.rel)
OBJ := $(OBJ:.s=.rel)

LIBPATH := ../lib/
INCPATH := ../include/

all: ${TARGET}.lib

${TARGET}.lib: ${OBJ}
	@$(AR) -q "$@" $^

%.rel: %.c
	$(CC) ${CFLAGS} -c "$<"

%.rel: %.s
	$(AS) -l -o -s "$<"

install: ${TARGET}.lib
	mkdir -p ${LIBPATH}
	cp ${TARGET}.lib ${LIBPATH}

install-headers: *.h
	mkdir -p ${INCPATH}/${TARGET}/
	cp *.h ${INCPATH}/${TARGET}/

clean:
	@rm -f *.lib *.lk *.map *.mem *.lst *.rel *.rst *.sym *.asm *.ihx *.noi *.hex
	@rm -f ${LIBPATH}/${TARGET}.lib
	@rm -rf ${INCPATH}/${TARGET}
