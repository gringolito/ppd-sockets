#***********************************************************************
#
#  "THE BEER-WARE LICENSE" (Revision 42):
#  <filipeutzig@gmail.com> wrote this file. As long as you retain this
#  notice you can do whatever you want with this stuff. If we meet some
#  day, and you think this stuff is worth it, you can buy me a beer in
#  return.
#
#  Initial version by Filipe Utzig <filipeutzig@gmail.com> on 3/24/15.
#
#***********************************************************************

CC=cc
CFLAGS=-Wall -Wextra -pipe -Og -g3
#CFLAGS=-Wall -Wextra -pipe -O2 -march=native
LDFLAGS=
EXTRA_CFLAGS=-DPROG_NAME=\"$(TARGET)\"
DEBUG=no
LIB_PATH=../../lib
LIB_SRC=$(wildcard $(LIB_PATH)/*.c)
LIBS=$(LIB_SRC:.c=.o)
SRC=$(wildcard *.c)
OBJ=$(LIBS) $(SRC:.c=.o)
INC_PATH=-I../../include
TARGET=client
PREFIX=../..
INSTALL=install -m 0755

ifeq (yes,$(DEBUG))
override EXTRA_CFLAGS += -DDEBUG
endif
ifeq (YES,$(DEBUG))
override EXTRA_CFLAGS += -DDEBUG
endif
ifeq (1,$(DEBUG))
override EXTRA_CFLAGS += -DDEBUG
endif

.PHONY  : all default install clean mrproper $(TARGET)

default : clean $(TARGET)

all     : $(TARGET)

$(TARGET): $(OBJ)
	@$(CC) $(OBJ) $(LDFLAGS) -o $@
	@echo "[LD] $(OBJ) -> $@"

%.o     : %.c
	@$(CC) -o $@ -c $< $(CFLAGS) $(EXTRA_CFLAGS) $(INC_PATH)
	@echo "[CC] $<"

install : $(TARGET)
	@test -d $(PREFIX) || mkdir $(PREFIX)
	@test -d $(PREFIX)/bin || mkdir $(PREFIX)/bin
	@$(INSTALL) $(TARGET) $(PREFIX)/bin

clean   :
	@$(RM) *.o *~ $(TARGET)

mrproper: clean
	@cd $(PREFIX)/bin && $(RM) $(TARGET)

