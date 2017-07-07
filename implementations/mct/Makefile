# List of subdirectories
SUBDIRS  = closed
SUBDIRS += linked

# Do not edit anything below, make creates it.
# -#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#

# The project's name
PROJECT    = rht

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# The project's tree
ROOTDIR    = /home/rocco/${PROJECT}
SHAREDIR   = ${ROOTDIR}/share
INSTALLDIR = ${ROOTDIR}/lib

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

INCLUDE  += -I${RSUITEDIR}/lib
RSUITELIB = ${RSUITEDIR}/lib/librhash-ut.a
USRLIBS  += ${RUTLIB}

# -#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#
PROJECT   = rht
ROOTDIR   = /home/rocco/${PROJECT}

# The project's tree
ROOTDIR    = /home/rocco/${PROJECT}
SHAREDIR   = ${ROOTDIR}/share
INSTALLDIR = ${ROOTDIR}/lib

RLIBCDIR   = ${ROOTDIR}/rlibc
RUTDIR     = ${ROOTDIR}/rut
RSUITEDIR  = ${ROOTDIR}/tests/suite

# rlibc
INCLUDE  += -I${RLIBCDIR}
RLIBCLIB  = ${RLIBCDIR}/librlibc.a
USRLIBS  += ${RLIBCLIB}

# rut
INCLUDE  += -I${RUTDIR}
RUTLIB    = ${RUTDIR}/librut.a
USRLIBS  += ${RUTLIB}

# rut
INCLUDE  += -I${RSUITEDIR}/lib
RSUITELIB = ${RSUITEDIR}/lib/librhash-ut.a
USRLIBS  += ${RUTLIB}


TEMPLATES = ${SHAREDIR}/PROJECT ${SHAREDIR}/FileSystem ${SHAREDIR}/Makefile.subdirs
PLEASE    = "Do not edit anything below, make creates it."

# The main target is responsible to recursively scan subdirectories and make all the defined targets
all: Makefile
	@for dir in ${SUBDIRS} ; do \
           if [ -d $$dir ] ; then \
             echo "Making $$dir ..." ; \
             (cd $$dir && make -s --no-print-directory) ; \
           else \
             echo "Warning: missing sub-directory $$dir" ; \
           fi \
         done

# Cleanup rules
clean distclean:
	@rm -f *~
	@for dir in ${SUBDIRS} ; do \
           if [ -d $$dir ] ; then \
             (cd $$dir && make -s --no-print-directory $@) ; \
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

# Fake targets
.PHONY: all clean distclean ${SUBDIRS}
