.RECIPEPREFIX = >
.DEFAULT_GOAL := all

# Options
BUILDDIR := build

#  Global build conf
CC= avr-gcc
OBJCOPY= avr-objcopy
CFLAGS= -O3 -I.
CFLAGS += -DF_CPU=13000000UL
MCFLAGS= -mmcu=atmega328p

include common/build_common.mk

PROGRAMS_MAIN = pov led_PD6 serial
BINS = $(addprefix $(BUILDDIR)/, $(addsuffix .bin,${PROGRAMS_MAIN}))

all: MK_BUILDDIR $(BINS)

MK_BUILDDIR:
> mkdir -p ${BUILDDIR}

$(BUILDDIR)/%.bin : $(BUILDDIR)/%.out
> ${OBJCOPY} -O binary $< $@

$(BUILDDIR)/%.out : %.c
> $(CC) $(CFLAGS) ${MCFLAGS} $< $(COMMON_SRCS) -o $@

$(addprefix flash_, ${PROGRAMS_MAIN}):
> avrdude -p m328p -c usbasp -U flash:w:$<

.PHONY: clean flash

clean:
> rm -rf $(BUILDDIR)
