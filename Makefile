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

include common/build_common.mk

ifeq (${ARDUINO},0)
  CFLAGS += -DF_CPU=13000000UL
  AVG_AVRDUDE= usbasp
else
  CFLAGS += -DF_CPU=16000000UL
  AVG_AVRDUDE= arduino -P ${DEVICE}
endif

PROGRAMS_MAIN = pov led_PB6 serial
BINS = $(addprefix $(BUILDDIR)/, $(addsuffix .bin,${PROGRAMS_MAIN}))

all: MK_BUILDDIR $(BINS)

MK_BUILDDIR:
> mkdir -p ${BUILDDIR}

$(BUILDDIR)/%.bin : $(BUILDDIR)/%.out
> ${OBJCOPY} -O binary $< $@

$(BUILDDIR)/%.out : %.c
> $(CC) $(CFLAGS) ${MCFLAGS} $< $(COMMON_SRCS) -o $@

$(addprefix flash_, ${PROGRAMS_MAIN}):
> avrdude -p m328p -c $(AVG_AVRDUDE) -U flash:w:$<

.PHONY: clean flash

clean:
> rm -rf $(BUILDDIR)
