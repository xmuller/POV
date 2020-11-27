.RECIPEPREFIX = >
.DEFAULT_GOAL := all

# Options
BUILDDIR := build
DEVICE ?= /dev/ttyACM0
ARDUINO ?= 0

#  Global build conf
CC= avr-gcc
OBJCOPY= avr-objcopy
CFLAGS= -O3 -I.
MCFLAGS= -mmcu=atmega328p

ifeq (${ARDUINO},0)
  CFLAGS += -DF_CPU=13000000UL
  AVG_AVRDUDE= usbasp
else
  CFLAGS += -DF_CPU=16000000UL
  AVG_AVRDUDE= arduino
endif


# TARGETS
all: ${BUILDDIR} ${BUILDDIR}/pov.bin

${BUILDDIR}:
> mkdir -p ${BUILDDIR}

${BUILDDIR}/pov.bin: ${BUILDDIR}/pov.out
> ${OBJCOPY} -O binary $< $@

${BUILDDIR}/pov.out:
> ${CC} ${CFLAGS} ${MCFLAGS} pov.c timer.c -o $@

flash: ${BUILDDIR}/pov.bin
> avrdude -p m328p -c ${AVG_AVRDUDE} -P ${DEVICE} -U flash:w:$<

.PHONY: clean flash

clean:
> rm -rf ${BUILDDIR}
