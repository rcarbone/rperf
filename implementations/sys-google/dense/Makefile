# The name of the game
NAME      = densehash
# C source files
LIBSRCS   = glue.c
MAINSRCS  = run-suite.c
#
INCLUDE   =
CFLAGS    =
C++FLAGS  =
SYSLIBS   = -lstdc++

# Do not edit anything below, make creates it.
# -#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#

# The project's name
PROJECT    = rperf

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# The project's tree
ROOTDIR    = /home/rocco/${PROJECT}

SHAREDIR   = ${ROOTDIR}/share
INSTALLDIR = ${ROOTDIR}/plugins

# The project's source tree
RLIBCDIR   = ${ROOTDIR}/rlibc
RHTDIR     = ${ROOTDIR}/rht

# rlibc
INCLUDE   += -I${RLIBCDIR}
RLIBCLIB   = ${RLIBCDIR}/librlibc.a
USRLIBS   += ${RLIBCLIB}

# rht
INCLUDE   += -I${RHTDIR}
RHTLIB     = ${RHTDIR}/librht.a
USRLIBS   += ${RHTLIB}

# -#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#

TEMPLATES = ${SHAREDIR}/PROJECT ${SHAREDIR}/FileSystem ${SHAREDIR}/Makefile.plugins ${SHAREDIR}/LIBS
PLEASE    = "Do not edit anything below, make creates it."

# Static and dynamic libraries
# STLIB     = librht-${NAME}.a
SHLIB     = ${NAME}.so

# All the binary programs
PROGRAMS  = run

# All the targets
TARGETS   = ${SHLIB} ${INSTALLDIR}/${SHLIB} ${PROGRAMS}

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
all: Makefile plugin.c run.c load.c ${TARGETS} ${RHASHLIB} ${RHTLIB}

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

run.c: ${SHAREDIR}/run.c
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
${SHLIB}: plugin.o glue.o ${RHTLIB} ${RLIBCLIB}
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
run: run.o glue.o ${RHTLIB} ${RLIBCLIB}
	@echo "=*= making program $@ =*="
	@${LD} ${LDFLAGS} $^ ${SYSLIBS} -o $@

load: load.o ${RLIBCLIB} ${STLIB}
	@echo "=*= making program $@ =*="
	@${LD} ${LDFLAGS} $^ ${SYSLIBS} -ldl -o $@

unit: run
	@run -x

suite: run
	@run -X

# Cleanup rules
clean:
	@rm -f ${TARGETS}
	@rm -f ${OBJS}
	@rm -f *~
	@rm -f plugin.o
	@rm -f run.o
	@rm -f load.o

distclean: clean
	@rm -f ${DEPS}
	@rm -f plugin.c
	@rm -f run.c
	@rm -f load.c

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
RUNFLAG   = -x

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
	@echo -n "  $^ ${RUNFLAG} "
	@valgrind -q ${VFLAGS} $^ ${RUNFLAG} 1> /dev/null 2>& 1
	@if [ $? ]; then echo -n "Ok"; fi
	@echo

vleak-%: %
	@echo "  $^ ${RUNFLAG} "
	@echo "valgrind ${VFLAGS} $^ ${RUNFLAG}"
	@valgrind ${VFLAGS} $^ ${RUNFLAG}

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

help:
	@echo "Usage:"
	@echo
	@echo "[all]"     | awk '{ printf "  make %-15.15s  --> Compile all the defined targets (default)\n", $$1 }'
	@echo ${STLIB}    | awk '{ printf "  make %-15.15s  --> Compile the static library\n", $$1 }'
#	@echo ${SHLIB}    | awk '{ printf "  make %-15.15s  --> Compile the shared library\n", $$1 }'
	@echo ${PROGRAMS} | awk '{ printf "  make %-15.15s  --> Compile the binary program\n", $$1 }'
	@echo
	@echo "clean"     | awk '{ printf "  make %-15.15s  --> Clean all generated files\n", $$1 }'
	@echo "distclean" | awk '{ printf "  make %-15.15s  --> Clean all generated files (included temporary)\n", $$1 }'
	@echo
	@echo "unit"      | awk '{ printf "  make %-15.15s  --> Run all the unit tests\n", $$1 }'
	@echo "suite"     | awk '{ printf "  make %-15.15s  --> Run the test suite\n", $$1 }'
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

# rht
${RHTLIB}:
	@echo "==================="
	@echo "=*= making ${RHTLIB} =*="
	@(cd ${RHTDIR} && make)
	@echo "==================="
