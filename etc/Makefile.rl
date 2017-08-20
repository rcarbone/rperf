TEMPLATES = ${ETCDIR}/FileSystem ${ETCDIR}/Makefile.rl ${ETCDIR}/LIBS

# Plugin (dynamic library)
SHLIB     = ${NAME}.so

# All the binary programs
PROGRAMS  = ut-run suite-run

# All the targets
TARGETS   = ${SHLIB} ${INSTALLDIR1}/${SHLIB} ${PROGRAMS}

# Source, object and depend files
CSRCS     = ${LIBCSRCS} ${LIBC++SRCS}
LIBOBJS   = ${LIBCSRCS:%.c=%.o} ${LIBC++SRCS:%.cpp=%.o}
DEPS      = ${LIBCSRCS:%.c=%.M} ${LIBC++SRCS:%.cpp=%.M}
OBJS      = ${LIBOBJS} ${MAINOBJS}

# C compiler and flags
INCLUDES  = -I./
INCLUDES += ${INCLUDE}
CC        = gcc
C++       = g++
AR        = ar
LD        = gcc
OPTIMIZE  = -O3
# OPTIMIZE += -flto -fuse-linker-plugin
CFLAGS   += -g -Wall
CFLAGS   += ${OPTIMIZE}
CFLAGS   += ${INCLUDES}
CFLAGS   += -fPIC
CFLAGS   += -fno-diagnostics-color
C++FLAGS += ${CFLAGS}
ARFLAGS   = rv
LDFLAGS   =

# System libraries
SYSLIBS  += -ldl

# The main target is responsible to make all the targets
all: Makefile plugin-rl.c ut-run.c suite-run.c ${INSTALLDIR1} ${TARGETS} ${RLIBCLIB} ${RLLIB}

Makefile: Makefile.in ${TEMPLATES}
	@ROOTDIR=`grep ROOTDIR Makefile`
	@echo "=*= making $@ =*="
	@cat Makefile.in                                        > Makefile
	@echo "# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" >> Makefile
	@echo "# Do not edit anything below, make creates it." >> Makefile
	@echo "# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" >> Makefile
	@echo                                                  >> Makefile
	@echo "# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" >> Makefile
	@echo "# The project's root directory"                 >> Makefile
	@echo "ROOTDIR    = ${ROOTDIR}"                        >> Makefile
	@echo -n "ETCDIR     = $$"                             >> Makefile
	@echo "{ROOTDIR}/etc"                                  >> Makefile
	@echo "# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" >> Makefile
	@echo                                                  >> Makefile
	@echo "# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" >> Makefile
	@echo                                                  >> Makefile
	@cat ${TEMPLATES}                                      >> Makefile

plugin-rl.c: README.c ${ETCDIR}/plugin-rl.c
	@echo "=*= making C source $@ =*="
	@cat $^ > $@

ut-run.c: ${RLDIR}/ut-run.c
	@echo "=*= making C source $@ =*="
	@cat $^ > $@

suite-run.c: ${RLDIR}/suite-run.c
	@echo "=*= making C source $@ =*="
	@cat $^ > $@

# Static Library
${STLIB}: ${LIBOBJS}
	@echo "=*= making static library $@ =*="
	@${AR} ${ARFLAGS} $@ $^ 1> /dev/null 2>& 1

# Dynamic Library
${SHLIB}: plugin-rl.o glue.o ${RLLIB} ${RLIBCLIB}
	@echo "=*= making plugin $@ =*="
	@${CC} -shared $^ ${SYSLIBS} -o $@

# Shared Library
${INSTALLDIR1}/${SHLIB}: ${SHLIB}
	@if [ -d ${INSTALLDIR1} ] ; then \
          (rm -f $@ ; cp $^ $@) ; \
         else \
          echo "Warning: plugin $@ not locally installed in ${INSTALLDIR1}" ; \
         fi

${INSTALLDIR1}:
	@if [ ! -d ${INSTALLDIR1} ] ; then \
           mkdir ${INSTALLDIR1} ; \
         fi

# Binary programs
ut-run: ut-run.o glue.o ${ROOTDIR}/src/lists/builtins/librl.a ${RLIBCLIB}
	@echo "=*= making program $@ =*="
	@${LD} ${LDFLAGS} $^ ${SYSLIBS} -o $@

suite-run: suite-run.o glue.o ${ROOTDIR}/src/lists/builtins/librl.a ${RLIBCLIB}
	@echo "=*= making program $@ =*="
	@${LD} ${LDFLAGS} $^ ${SYSLIBS} -o $@

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

unit: ut-run
	@./$^ -x

run: suite-run
	@./$^ -x

x:
	@echo "=*= running unit tests =*="
	@./ut-run -x
	@echo
	@echo "=*= running test suite =*="
	@./suite-run -x

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

# Cleanup rules
clean:
	@rm -f ${TARGETS}
	@rm -f ${OBJS}
	@rm -f *~
	@rm -f plugin-rl.o
	@rm -f ut-run.o
	@rm -f suite-run.o

distclean: clean
	@rm -f ${DEPS}
	@rm -f plugin-rl.c
	@rm -f ut-run.c
	@rm -f suite-run.c

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

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

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
	@if [ $? ] ; then echo -n "Ok" ; fi
	@echo

vleak-%: %
	@echo "  $^ ${RUNFLAG} "
	@echo "valgrind ${VFLAGS} $^ ${RUNFLAG}"
	@valgrind ${VFLAGS} $^ ${RUNFLAG}

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

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
	@echo "run"       | awk '{ printf "  make %-15.15s  --> Run the test suite\n", $$1 }'
	@echo "leaks"     | awk '{ printf "  make %-15.15s  --> Ckeck for errors and memory leaks\n", $$1 }'
	@echo "vleaks"    | awk '{ printf "  make %-15.15s  --> Ckeck for errors and memory leaks (verbose mode)\n", $$1 }'

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
# The fake targets
# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
.PHONY: all clean distclean leaks vleaks

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
# Optimization experimental
# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
SRCS = ${LIBCSRCS} ${LIBC++SRCS}

glue-gcc.o: glue.cpp
	@echo "=*= making object $@ =*="
	@g++ -c ${C++FLAGS} $< -o $@

glue-clang.o: glue.cpp
	@echo "=*= making object $@ =*="
	@clang++ -c ${C++FLAGS} -std=c++11 $< -o $@

suite-gcc-O0: suite.c glue.o ${RLLIB} ${RLIBCLIB}
	@echo "=*= making program $@ =*="
	@gcc ${INCLUDES} $^ ${SYSLIBS} -o $@

suite-gcc-O1: suite.c glue-gcc.o ${RLLIB} ${RLIBCLIB}
	@echo "=*= making program $@ =*="
	@gcc ${INCLUDES} -O1 $^ ${SYSLIBS} -o $@

suite-gcc-O2: suite.c glue-gcc.o ${RLLIB} ${RLIBCLIB}
	@echo "=*= making program $@ =*="
	@gcc ${INCLUDES} -O2 $^ ${SYSLIBS} -o $@

suite-gcc-O3: suite.c glue-gcc.o ${RLLIB} ${RLIBCLIB}
	@echo "=*= making program $@ =*="
	@gcc ${INCLUDES} -O3 $^ ${SYSLIBS} -o $@

suite-clang-O0: suite.c glue-clang.o ${RLLIB} ${RLIBCLIB}
	@echo "=*= making program $@ =*="
	@clang ${INCLUDES} $^ ${SYSLIBS} -o $@

suite-clang-O1: suite.c glue-clang.o ${RLLIB} ${RLIBCLIB}
	@echo "=*= making program $@ =*="
	@clang ${INCLUDES} -O1 $^ ${SYSLIBS} -o $@

suite-clang-O2: suite.c glue-clang.o ${RLLIB} ${RLIBCLIB}
	@echo "=*= making program $@ =*="
	@clang ${INCLUDES} -O2 $^ ${SYSLIBS} -o $@

suite-clang-O3: suite.c glue-clang.o ${RLLIB} ${RLIBCLIB}
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

# rl
${RLLIB}:
	@echo "==================="
	@echo "=*= making ${RLLIB} =*="
	@(cd ${RLDIR} && make)
	@echo "==================="
# Project libraries

# rlibc
${RLIBCLIB}:
	@echo "==================="
	@echo "=*= making ${RLIBCLIB} =*="
	@(cd ${RLIBCDIR} && make -s)
	@echo "==================="

# rl
${RLLIB}:
	@echo "==================="
	@echo "=*= making ${RLLIB} =*="
	@(cd ${RLDIR} && make)
	@echo "==================="

# rht
${RHTLIB}:
	@echo "==================="
	@echo "=*= making ${RHTLIB} =*="
	@(cd ${RHTDIR} && make)
	@echo "==================="
# Project libraries

# rlibc
${RLIBCLIB}:
	@echo "==================="
	@echo "=*= making ${RLIBCLIB} =*="
	@(cd ${RLIBCDIR} && make -s)
	@echo "==================="

# rl
${RLLIB}:
	@echo "==================="
	@echo "=*= making ${RLLIB} =*="
	@(cd ${RLDIR} && make)
	@echo "==================="

# rht
${RHTLIB}:
	@echo "==================="
	@echo "=*= making ${RHTLIB} =*="
	@(cd ${RHTDIR} && make)
	@echo "==================="
