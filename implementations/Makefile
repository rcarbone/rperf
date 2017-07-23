# List of subdirectories
SUBDIRS  = pm
SUBDIRS += amtl
SUBDIRS += apr
SUBDIRS += c-algorithms
SUBDIRS += clark
SUBDIRS += ccan
SUBDIRS  = eastl
SUBDIRS += emilib
SUBDIRS += gcc
SUBDIRS += google-c
SUBDIRS += hashit
SUBDIRS += klib
SUBDIRS += libCDS
SUBDIRS += libcfu
SUBDIRS += libevent
SUBDIRS += libghthash
SUBDIRS += mct
SUBDIRS += oddou
SUBDIRS += python
SUBDIRS += rdestl
SUBDIRS += rigtorp
SUBDIRS += st
SUBDIRS += stb
SUBDIRS += sys-apr
SUBDIRS += sys-boost
SUBDIRS += sys-glib
SUBDIRS += sys-google
SUBDIRS += sys-libstdc++
SUBDIRS += sys-Qt
SUBDIRS += sys-tbb
SUBDIRS += tommyds
SUBDIRS += ulib
SUBDIRS += uthash
# SUBDIRS += z-todo

# Do not edit anything below, make creates it.
# -#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#

# The project's name
PROJECT    = rperf

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# The project's source tree
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
TEMPLATES = ${SHAREDIR}/PROJECT ${SHAREDIR}/FileSystem ${SHAREDIR}/Makefile.subdirs
PLEASE    = "Do not edit anything below, make creates it."

# The main target is responsible to recursively scan subdirectories and make all the defined targets
#
# The macro SUBDIRS defines the ordered list of all the sub-directories that are recursively tried.
#
all: Makefile
	@for dir in ${SUBDIRS} ; do \
           if [ -d $$dir ] ; then \
             echo "Making $$dir ..." ; \
             (make -C $$dir -s --no-print-directory) ; \
           else \
             echo "Warning: missing sub-directory $$dir" ; \
           fi \
         done

# Cleanup rules
clean distclean:
	@rm -f *~
	@for dir in ${SUBDIRS} ; do \
           if [ -d $$dir ] ; then \
             (make -C $$dir -s --no-print-directory $@) ; \
           fi \
         done

# The targets that are eventually handled by recursive makefile
%:
	@for dir in ${SUBDIRS} ; do \
           if [ -d $$dir ] ; then \
             echo $$dir; \
             (make -C $$dir -s --no-print-directory $@) ; \
             echo ; \
           fi \
         done

Makefile: ${TEMPLATES}
	@echo "=*= making $@ =*="
	@cat $@ | head -`grep -n ${PLEASE} $@ | head -1 | cut -d ':' -f 1` > $@.tmp
	@echo "# -#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#" >> $@.tmp
	@echo >> $@.tmp
	@cat $^ >> $@.tmp
	@mv $@.tmp $@

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# The fake targets
# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
.PHONY: all clean distclean ${SUBDIRS}

# In the event an unknown rule has been used
.DEFAULT:
	@true
