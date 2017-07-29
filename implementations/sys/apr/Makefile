# The name of the game
NAME      = apr

# C/C++ source files
LIBSRCS   = glue.c
MAINSRCS  = run-suite.c

# Optional
INCLUDE   =
CFLAGS    =
C++FLAGS  =
SYSLIBS   = -lapr-1 -lpthread

# Do not edit anything below, make creates it.
# -#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#

# The project's name
PROJECT    = rperf

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# The project's source tree
ROOTDIR = /home/rocco/git/rcarbone/rperf

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

# Plugin (dynamic library)
SHLIB     = ${NAME}.so

# All the binary programs
PROGRAMS  = ut suite

# All the targets
TARGETS   = ${SHLIB} ${INSTALLDIR}/${SHLIB} ${PROGRAMS}

# Source, Object and depend files
CSRCS     = ${LIBCSRCS} ${LIBC++SRCS}
LIBOBJS   = $(LIBCSRCS:%.c=%.o) $(LIBC++SRCS:%.cpp=%.o)
MAINOBJS  = $(LIBCSRCS:%.c=%.o) $(LIBC++SRCS:%.cpp=%.o)
DEPS      = $(LIBCSRCS:%.c=%.M) $(LIBC++SRCS:%.cpp=%.M)
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
all: Makefile plugin.c ut.c suite.c ${INSTALLDIR} ${TARGETS} ${RLIBCLIB} ${RHTLIB}

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

ut.c: ${SHAREDIR}/ut.c
	@echo "=*= making C source $@ =*="
	@cat $^ > $@

suite.c: ${SHAREDIR}/suite.c
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
	@if [ -d ${INSTALLDIR} ] ; then \
          (rm -f $@ ; cp $^ $@) ; \
         else \
          echo "Warning: plugin $@ not locally installed in ${INSTALLDIR}" ; \
         fi

${INSTALLDIR}:
	@if [ ! -d ${INSTALLDIR} ] ; then \
           mkdir ${INSTALLDIR} ; \
         fi

# Binary programs
ut: ut.o glue.o ${RHTLIB} ${RLIBCLIB}
	@echo "=*= making program $@ =*="
	@${LD} ${LDFLAGS} $^ ${SYSLIBS} -o $@

suite: suite.o glue.o ${RHTLIB} ${RLIBCLIB}
	@echo "=*= making program $@ =*="
	@${LD} ${LDFLAGS} $^ ${SYSLIBS} -o $@

unit: ut
	@./$^ -x

run: suite
	@./$^ -x

x:
	@echo "=*= running unit tests =*="
	@./ut -x
	@echo
	@echo "=*= running test suite =*="
	@./suite -x

# Cleanup rules
clean:
	@rm -f ${TARGETS}
	@rm -f ${OBJS}
	@rm -f *~
	@rm -f plugin.o
	@rm -f ut.o
	@rm -f suite.o

distclean: clean
	@rm -f ${DEPS}
	@rm -f plugin.c
	@rm -f ut.c
	@rm -f suite.c

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
	@-${CC} -MM ${CFLAGS} $< -o $@

# How to make a depend file from a C++ source
%.M: %.cpp
	@-${C++} -MM ${C++FLAGS} $< -o $@

-include ${DEPS}

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

# Check for errors and memory leaks

VFLAGS    = --show-leak-kinds=all
VFLAGS   += --leak-check=full
VFLAGS   += --error-exitcode=1
VVFLAGS   = ${VFLAGS}
VVFLAGS  += --show-reachable=yes
VVFLAGS  += --error-limit=no
RUNFLAG   = -X

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
	@if [ $? ] ; then echo -n "Ok" ; fi
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

.PHONY: all clean distclean

SRCS = ${LIBCSRCS} ${LIBC++SRCS}

glue-gcc.o: glue.cpp
	@echo "=*= making object $@ =*="
	@g++ -c ${C++FLAGS} $< -o $@

glue-clang.o: glue.cpp
	@echo "=*= making object $@ =*="
	@clang++ -c ${C++FLAGS} -std=c++11 $< -o $@

suite-gcc-O0: suite.c glue.o ${RHTLIB} ${RLIBCLIB}
	@echo "=*= making program $@ =*="
	@gcc ${INCLUDES} $^ ${SYSLIBS} -o $@

suite-gcc-O1: suite.c glue-gcc.o ${RHTLIB} ${RLIBCLIB}
	@echo "=*= making program $@ =*="
	@gcc ${INCLUDES} -O1 $^ ${SYSLIBS} -o $@

suite-gcc-O2: suite.c glue-gcc.o ${RHTLIB} ${RLIBCLIB}
	@echo "=*= making program $@ =*="
	@gcc ${INCLUDES} -O2 $^ ${SYSLIBS} -o $@

suite-gcc-O3: suite.c glue-gcc.o ${RHTLIB} ${RLIBCLIB}
	@echo "=*= making program $@ =*="
	@gcc ${INCLUDES} -O3 $^ ${SYSLIBS} -o $@

suite-clang-O0: suite.c glue-clang.o ${RHTLIB} ${RLIBCLIB}
	@echo "=*= making program $@ =*="
	@clang ${INCLUDES} $^ ${SYSLIBS} -o $@

suite-clang-O1: suite.c glue-clang.o ${RHTLIB} ${RLIBCLIB}
	@echo "=*= making program $@ =*="
	@clang ${INCLUDES} -O1 $^ ${SYSLIBS} -o $@

suite-clang-O2: suite.c glue-clang.o ${RHTLIB} ${RLIBCLIB}
	@echo "=*= making program $@ =*="
	@clang ${INCLUDES} -O2 $^ ${SYSLIBS} -o $@

suite-clang-O3: suite.c glue-clang.o ${RHTLIB} ${RLIBCLIB}
	@echo "=*= making program $@ =*="
	@clang ${INCLUDES} -O3 $^ ${SYSLIBS} -o $@

GCCALL   = suite-gcc-O0 suite-gcc-O1 suite-gcc-O2 suite-gcc-O3
CLANGALL = suite-clang-O0 suite-clang-O1 suite-clang-O2 suite-clang-O3
ALLO     = suite-gcc-O0 suite-clang-O0 suite-gcc-O1 suite-clang-O1 suite-gcc-O2 suite-clang-O2 suite-gcc-O3 suite-clang-O3

o: glue-gcc.o glue-clang.o ${ALLO}
	@for t in `seq 1 6` ; do \
           echo "Running test $$t" ; \
           for o in ${ALLO} ; do \
             echo -n "`echo $$o | cut -d '-' -f2-3`" | awk '{ printf "%-8.8s ", $$1 }' ; \
             ./$$o -x -6 -i $$t ; \
           done ; \
           echo "" ; \
         done

a: glue-gcc.o glue-clang.o ${ALLO}
	@for t in `seq 1 6` ; do \
           echo "Running test $$t" ; \
           for o in ${GCCALL} ${CLANGALL} ; do \
             echo -n "`echo $$o | cut -d '-' -f2-3`" | awk '{ printf "%-8.8s ", $$1 }' ; \
             ./$$o -x -6 -i $$t ; \
           done ; \
           echo "" ; \
         done

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
