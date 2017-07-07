# The name of the game
NAME      = hashit-open
# C source files
LIBSRCS   = glue.c
MAINSRCS  = run-suite.c
#
INCLUDE   = -I../3rdparty
CFLAGS    =
C++FLAGS  =
SYSLIBS   =

# Do not edit anything below, make creates it.
# -#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#

# The project's name
PROJECT    = rperf

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# The project's tree
ROOTDIR    = /home/rocco/${PROJECT}
SHAREDIR   = ${ROOTDIR}/share
INSTALLDIR = ${ROOTDIR}/plugins

RLIBCDIR   = ${ROOTDIR}/rlibc
RTESTDIR   = ${ROOTDIR}/rtest
RSUITEDIR  = ${ROOTDIR}/tests/suite

# rlibc
INCLUDE  += -I${RLIBCDIR}
RLIBCLIB  = ${RLIBCDIR}/librlibc.a
USRLIBS  += ${RLIBCLIB}

# rtest
INCLUDE  += -I${RTESTDIR}
RTESTLIB  = ${RTESTDIR}/librtest.a
USRLIBS  += ${RTESTLIB}

# -#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#

TEMPLATES = ${SHAREDIR}/PROJECT ${SHAREDIR}/FileSystem ${SHAREDIR}/Makefile.plugins ${SHAREDIR}/LIBS
PLEASE    = "Do not edit anything below, make creates it."

# Static and dynamic libraries
STLIB     = libr-${NAME}.a
SHLIB     = ${NAME}.so

# All the binary programs
PROGRAMS  = rut
PROGRAMS += rts

# All the targets
TARGETS   = ${STLIB} ${SHLIB} ${INSTALLDIR}/${SHLIB} ${PROGRAMS}

# Object and depend files
LIBOBJS   = $(patsubst %.c,%.o, ${LIBSRCS})
MAINOBJS  = $(patsubst %.c,%.o, ${MAINSRCS})
DEPS      = $(patsubst %.c,%.M, ${LIBSRCS} ${MAINSRCS})
OBJS      = ${LIBOBJS} ${MAINOBJS}

# C compiler and flags
INCLUDES  = -I./
INCLUDES += ${INCLUDE}
CC        = gcc
C++       = g++
AR        = ar
LD        = gcc
# OPTIMIZE  = -O3
CFLAGS   += -g -Wall
# CFLAGS   += ${OPTIMIZE}
CFLAGS   += ${INCLUDES}
CFLAGS   += -fPIC
CFLAGS   += -fno-diagnostics-color
C++FLAGS += ${CFLAGS}
ARFLAGS   = rv
LDFLAGS   =

# The main target is responsible to make all the targets
all: Makefile plugin.c rut.c rts.c load.c ${TARGETS} ${RHASHLIB} ${RUTLIB}

Makefile: ${TEMPLATES}
	@echo "=*= making $@ =*="
	@cat $@ | head -`grep -n ${PLEASE} $@ | head -1 | cut -d ':' -f 1` > $@.tmp
	@echo "# -#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#" >> $@.tmp
	@echo >> $@.tmp
	@cat $^ >> $@.tmp
	@mv $@.tmp $@

plugin.c: README.c ${SHAREDIR}/plugin.c
	@echo "=*= making C source $@ =*="
	@cat $^ > $@

rut.c: ${SHAREDIR}/rut.c
	@echo "=*= making C source $@ =*="
	@cat $^ > $@

rts.c: ${SHAREDIR}/rts.c
	@echo "=*= making C source $@ =*="
	@cat $^ > $@

load.c: ${SHAREDIR}/load.c
	@echo "=*= making C source $@ =*="
	@cat $^ > $@

# Static Library
${STLIB}: ${LIBOBJS}
	@echo "=*= making static library $@ =*="
	@${AR} ${ARFLAGS} $@ $^ 1> /dev/null 2>& 1

# Dynamic Library
${SHLIB}: plugin.o ${RTESTLIB} ${RLIBCLIB} ${STLIB}
	@echo "=*= making plugin $@ =*="
	@${CC} -shared $^ ${SYSLIBS} -o $@

# Shared Library
${INSTALLDIR}/${SHLIB}: ${SHLIB}
	@if [ -d ${INSTALLDIR} ]; then \
          (rm -f $@; cp $^ $@) ; \
         else \
          echo "Warning: plugin $@ not locally installed in ${INSTALLDIR}"; \
         fi

# Binary programs
rut: rut.o ${RTESTLIB} ${RLIBCLIB} ${STLIB}
	@echo "=*= making program $@ =*="
	@${LD} ${LDFLAGS} $^ ${SYSLIBS} -o $@

rts: rts.o ${RTESTLIB} ${RLIBCLIB} ${STLIB}
	@echo "=*= making program $@ =*="
	@${LD} ${LDFLAGS} $^ ${SYSLIBS} -o $@

load: load.o ${RLIBCLIB} ${STLIB}
	@echo "=*= making program $@ =*="
	@${LD} ${LDFLAGS} $^ ${SYSLIBS} -o $@

run: rut
	@$^ -x

# Cleanup rules
clean:
	@rm -f ${TARGETS}
	@rm -f ${OBJS}
	@rm -f *~
	@rm -f plugin.o
	@rm -f rut.o
	@rm -f rts.o
	@rm -f load.o

distclean: clean
	@rm -f ${DEPS}

# How to make an object file from a C source
%.o: %.c
	@echo "=*= making object $@ =*="
	@${CC} -c ${CFLAGS} $<

# How to make an object file from a C++ source
%.o: %.cpp
	@echo "=*= making object $@ =*="
	@${C++} -c ${C++FLAGS} $<

# How to make a depend file from a C source
%.M: %.c
	@-${CC} -MM ${INCLUDES} $< -o $@

# How to make a depend file from a C++ source
%.M: %.cpp
	@-${C++} -MM ${INCLUDES} $< -o $@

-include ${DEPS}

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

# Check for errors and memory leaks

VFLAGS    = --show-leak-kinds=all
VFLAGS   += --leak-check=full
VFLAGS   += --error-exitcode=1
VVFLAGS   = ${VFLAGS}
VVFLAGS  += --show-reachable=yes
VVFLAGS  += --error-limit=no

# Run valgrind to check for errors and memory leaks
leaks: ${PROGRAMS}
	@echo "=*= ckecking programs for errors with valgrind =*="
	@for bin in ${PROGRAMS} ; do \
           make -s leak-$$bin ; \
         done

vleaks: ${PROGRAMS}
	@echo "=*= ckecking programs for errors with valgrind =*="
	@for bin in ${PROGRAMS} ; do \
           make -s vleak-$$bin ; \
         done

leak-%: %
	@echo -n "  $^ "
	@valgrind -q ${VFLAGS} $^ 1> /dev/null 2>& 1
	@if [ $? ]; then echo -n "Ok"; fi
	@echo

vleak-%: %
	@echo "  $^ "
	@echo "valgrind ${VFLAGS} $^"
	@valgrind ${VFLAGS} $^

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

help:
	@echo "Usage:"
	@echo
	@echo "[all]"     | awk '{ printf "  make %-15.15s  --> Compile all the defined targets\n", $$1 }'
	@echo ${LIB}      | awk '{ printf "  make %-15.15s  --> Compile the static library\n", $$1 }'
	@echo ${SHLIB}    | awk '{ printf "  make %-15.15s  --> Compile the shared library\n", $$1 }'
	@echo ${PROGRAM}  | awk '{ printf "  make %-15.15s  --> Compile the binary program\n", $$1 }'
	@echo
	@echo "clean"     | awk '{ printf "  make %-15.15s  --> Clean all generated files\n", $$1 }'
	@echo "distclean" | awk '{ printf "  make %-15.15s  --> Clean all generated files (included temporary)\n", $$1 }'
	@echo "run"       | awk '{ printf "  make %-15.15s  --> Run the binary program\n", $$1 }'
	@echo "leaks"     | awk '{ printf "  make %-15.15s  --> Ckeck for errors and memory leaks\n", $$1 }'
	@echo "vleaks"    | awk '{ printf "  make %-15.15s  --> Ckeck for errors and memory leaks (verbose mode)\n", $$1 }'

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

# Project libraries

# rlibc
${RLIBCLIB}:
	@echo "==================="
	@echo "=*= making ${RLIBCLIB} =*="
	@(cd ${RLIBCDIR} && make -s)
	@echo "==================="

# rut
${RUTLIB}:
	@echo "==================="
	@echo "=*= making ${RUTLIB} =*="
	@(cd ${RUTDIR} && make)
	@echo "==================="
