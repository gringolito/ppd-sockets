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

#
# You probably don't need to modify anything in this Makefile.
#

APPS:=sequential parallel
LIBS:=lib
DIRS:=src/sequential src/parallel lib

.PHONY : $(APPS) $(LIBS) mpi install clean

all    : $(APPS)

libs   : $(LIBS)

$(LIBS):
	@$(MAKE) -C $@

$(APPS): $(LIBS)
	@$(MAKE) -C src/$@

mpi    : parallel

install: $(APPS)
	@for dir in $(APPS); do \
		$(MAKE) -C src/$$dir install; \
	done

clean:
	@for dir in $(DIRS); do \
		$(MAKE) -C $$dir clean; \
	done
	@$(RM) -f var/log/*

