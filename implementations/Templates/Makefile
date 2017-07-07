# The name of the game
NAME      = XXX
# C source files
LIBSRCS   = glue.c
MAINSRCS  = run-suite.c
#
INCLUDE   = -I./3rdparty
CFLAGS    =
C++FLAGS  =
SYSLIBS   =

# Do not edit anything below, make creates it.
# -#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#

ROOTDIR   = /home/rocco/rbench
SHAREDIR  = ${ROOTDIR}/share
RHASHDIR  = ${ROOTDIR}
RSUITEDIR = ${ROOTDIR}/tests/suite

TEMPLATE  = ${SHAREDIR}/Makefile.plugins

LIB       = libr-${NAME}.a
SHLIB     = ${NAME}.so
PROGRAM   = run-testsuite
PROGRAMS  = ${PROGRAM}
TARGETS   = ${LIB} ${SHLIB} ${PROGRAMS}

# librhash - an abstract C library over real hash table implementations 

RHASHINC  = ${RHASHDIR}/include
RHASHLIB  = ${RHASHDIR}/lib/librhash.a
RSUITEINC = ${RSUITEDIR}/include
RSUITELIB = ${RSUITEDIR}/lib/librhash-suite.a

# Object and depend files
LIBOBJS   = $(patsubst %.c,%.o, ${LIBSRCS})
MAINOBJS  = $(patsubst %.c,%.o, ${MAINSRCS})
DEPS      = $(patsubst %.c,%.M, ${LIBSRCS} ${MAINSRCS})
OBJS      = ${LIBOBJS} ${MAINOBJS}

# C compiler and flags
INCLUDES  = ${INCLUDE}
INCLUDES += -I${RHASHINC}
INCLUDES += -I${RSUITEINC}
INCLUDES += -I${RSUITEDIR}/lib
CC        = gcc
C++       = g++
AR        = ar
LD        = gcc
CFLAGS   += -g -Wall
CFLAGS   += ${INCLUDES}
CFLAGS   += -fPIC
CFLAGS   += -fno-diagnostics-color
C++FLAGS += ${CFLAGS}
ARFLAGS   = rv
LDFLAGS   =

# Static private libraries
USRLIBS   = ${RSUITELIB}
USRLIBS  += ${RHASHLIB}
USRLIBS  += ${LIB}

# The main target is responsible to make all the targets
all: Makefile plugin.c run-suite.c ${RHASHLIB} ${RSUITELIB} ${TARGETS}

PLEASE = "Do not edit anything below, make creates it."

Makefile: ${TEMPLATE}
	@echo "=*= making $@ =*="
	@cat $@ | head -`grep -n ${PLEASE} $@ | head -1 | cut -d ':' -f 1` > $@.tmp
	@echo "# -#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#" >> $@.tmp
	@echo >> $@.tmp
	@cat $^ >> $@.tmp
	@mv $@.tmp $@

plugin.c: README.c ${SHAREDIR}/plugin.c
	@echo "=*= making C source $@ =*="
	@cat $^ > $@

run-suite.c: ${SHAREDIR}/run-suite.c
	@echo "=*= making C source $@ =*="
	@cat $^ > $@

# Required library
${RHASHLIB}:
	@echo "==================="
	@echo "=*= making ${RHASHLIB} =*="
	@(cd ${RHASHDIR} && make -s)
	@echo "==================="

${RSUITELIB}:
	@echo "==================="
	@echo "=*= making ${RSUITELIB} =*="
	@(cd ${RSUITEDIR} && make -s)
	@echo "==================="

# Static Library
${LIB}: ${LIBOBJS}
	@echo "=*= making static library $@ =*="
	@${AR} ${ARFLAGS} $@ $^ 1> /dev/null 2>& 1

# Shared Object
${SHLIB}: plugin.o
	@echo "=*= making plugin $@ =*="
	@${CC} -shared $^ ${USRLIBS} ${SYSLIBS} -o $@

# Binary program
${PROGRAM}: ${MAINOBJS} ${USRLIBS}
	@echo "=*= making program $@ =*="
	@${LD} ${LDFLAGS} $^ ${SYSLIBS} -o $@

run: ${PROGRAM}
	@${PROGRAM}

# Cleanup rules
clean:
	@rm -f ${TARGETS}
	@rm -f ${OBJS}
	@rm -f *~
	@rm -f plugin.o

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

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

# Memory leaks

VFLAGS    = --show-leak-kinds=all
VFLAGS   += --leak-check=full
VFLAGS   += --error-exitcode=1
VVFLAGS   = ${VFLAGS}
VVFLAGS  += --show-reachable=yes
VVFLAGS  += --error-limit=no

# Run valgrind to check for all errors and memory leaks
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

-include ${DEPS}
