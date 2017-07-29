# The name of the games
LIB       = rspeed
PROGRAM   = rspeed

# C source files
MAINSRCS += main.c
MAINSRCS += fake-rht.c
LIBSRCS  += run.c
LIBSRCS  += print.c

# Compiler flags and System libraries (optional)
INCLUDE   =
CFLAGS    =
C++FLAGS  =
SYSLIBS   = -ldl

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

# The project's name
PROJECT    = rperf

# The project's source tree
PROJECT    = rperf
ROOTDIR    = /home/rocco/git/rcarbone/${PROJECT}

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

# Do not edit anything below, make creates it.
# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

TEMPLATES = ${SHAREDIR}/PROJECT ${SHAREDIR}/FileSystem ${SHAREDIR}/Makefile.leaf

# Static library
STLIB     = lib${LIB}.a

# All the targets
TARGETS   = ${STLIB} ${PROGRAM}

# Source, object and depend files
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
OPTIMIZE  = -O3
CFLAGS   += -g -Wall
CFLAGS   += ${OPTIMIZE}
CFLAGS   += ${INCLUDES}
CFLAGS   += -fPIC
CFLAGS   += -fno-diagnostics-color
C++FLAGS += ${CFLAGS}
ARFLAGS   = rv
LDFLAGS   =

# The main target is responsible to make all the targets
all: Makefile ${TARGETS}

Makefile: Makefile.in ${TEMPLATES}
	@echo "=*= making $@ =*="
	@cat $^ >> $@.tmp
	@mv $@.tmp $@

# Static Library
${STLIB}: ${LIBOBJS}
	@echo "=*= making static library $@ =*="
	@${AR} ${ARFLAGS} $@ $^ 1> /dev/null 2>& 1

# Dynamic Library
${SHLIB}:
	@echo "=*= making shared library $@ =*="
	@${CC} -shared $^ ${USRLIBS} ${SYSLIBS} -o $@

# Binary programs
${PROGRAM}: ${MAINOBJS} ${STLIB} ${RHTLIB} ${RLIBCLIB}
	@echo "=*= making program $@ =*="
	@${LD} ${LDFLAGS} $^ ${SYSLIBS} -o $@

# Cleanup rules
clean:
	@rm -f ${TARGETS}
	@rm -f ${OBJS}
	@rm -f *~

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

# Everything else
.DEFAULT:
	@true
