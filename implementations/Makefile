# List of subdirectories
SUBDIRS  = pm
SUBDIRS += amtl
SUBDIRS += apr
SUBDIRS += c-algorithms
SUBDIRS += clark
# SUBDIRS += ccan
# SUBDIRS  = eastl
SUBDIRS += gcc
SUBDIRS += google-c
SUBDIRS += hashit
SUBDIRS += klib
SUBDIRS += libCDS
SUBDIRS += libcfu
# SUBDIRS += libevent
SUBDIRS += mct
SUBDIRS += oddou
SUBDIRS += python
SUBDIRS += rdestl
SUBDIRS += rigtorp
SUBDIRS += sys-apr
SUBDIRS += sys-boost
SUBDIRS += sys-glib
SUBDIRS += sys-google
SUBDIRS += sys-libstdc++
# SUBDIRS += sys-qt
# SUBDIRS += tommyds
SUBDIRS += ulib
# SUBDIRS += uthash
# SUBDIRS += z-experimental

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

tar:
	@echo -n "Making distribution/backup file ${PROJECT}.tar.gz ..."
	@(cd ${ROOTDIR} && make -s distclean && cd .. && tar cfz ${PROJECT}.tar.gz ${PROJECT} && mv ${PROJECT}.tar.gz ${PROJECT}/)
	@echo "Done"

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# The fake targets
# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
.PHONY: all clean distclean ${SUBDIRS}

# In the event of unknown rules
.DEFAULT:
	@true
