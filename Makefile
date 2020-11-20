.RECIPEPREFIX = >
.DEFAULT_GOAL := all

# Options
BUILDDIR := build
DEVICE ?= /dev/ttyACM0

#  Global build conf
CC= avr-gcc
OBJCOPY= avr-objcopy
CFLAGS= -O3 -DF_CPU=13000000UL
LDFLAGS=
MCFLAGS= -mmcu=atmega328p

# TARGETS

all: ${BUILDDIR} ${BUILDDIR}/pov.bin

${BUILDDIR}:
> mkdir -p ${BUILDDIR}


## led
${BUILDDIR}/pov.bin: ${BUILDDIR}/pov.out
> ${OBJCOPY} -O binary $< $@

${BUILDDIR}/pov.out:
> ${CC} ${CFLAGS} ${MCFLAGS} pov.c -o $@

flash: ${BUILDDIR}/pov.bin
> avrdude -p m328p -c usbasp -P ${DEVICE} -U flash:w:${BUILDDIR}/pov.bin

.PHONY: clean flash

clean:
> rm -rf ${BUILDDIR}
