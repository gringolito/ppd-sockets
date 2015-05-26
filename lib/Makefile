#***********************************************************************
#
#  "THE BEER-WARE LICENSE" (Revision 42):
#  <filipeutzig@gmail.com> wrote this file. As long as you retain this
#  notice you can do whatever you want with this stuff. If we meet some
#  day, and you think this stuff is worth it, you can buy me a beer in
#  return.
#
#  Initial version by Filipe Utzig <filipeutzig@gmail.com> on 3/19/15.
#
#***********************************************************************

CFLAGS=-Wall -Wextra -pipe -Og -g3
#CFLAGS=-Wall -Wextra -pipe -O2 -march=native
LDFLAGS=
INC_PATH=-I../include
DEBUG=yes
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

ifeq (yes,$(DEBUG))
override EXTRA_CFLAGS += -DDEBUG
endif
ifeq (YES,$(DEBUG))
override EXTRA_CFLAGS += -DDEBUG
endif
ifeq (1,$(DEBUG))
override EXTRA_CFLAGS += -DDEBUG
endif

.PHONY: all default clean

default:    clean $(OBJ)

all:        $(OBJ)

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS) $(EXTRA_CFLAGS) $(INC_PATH)
	@echo "[CC] $<"

clean:
	@$(RM) *.o *~

