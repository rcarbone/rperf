# The macro SUBDIRS defines the ordered list of all the sub-directories that are recursively tried.
SUBDIRS += align
SUBDIRS += align++
SUBDIRS += chain
SUBDIRS += chain++
SUBDIRS += open
SUBDIRS += open++
# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

# The project's source tree
ROOTDIR    = /home/rocco/git/rcarbone/rperf

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

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

TEMPLATES = ${SHAREDIR}/FileSystem ${SHAREDIR}/Makefile.node

# The main target is responsible to recursively scan subdirectories and make all the defined targets
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

Makefile: Makefile.in ${TEMPLATES}
	@echo "=*= making $@ =*="
	@cat $^ >> $@.tmp
	@mv $@.tmp $@

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# The fake targets
# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
.PHONY: all clean distclean ${SUBDIRS}

# In the event an unknown rule has been used
.DEFAULT:
	@true
