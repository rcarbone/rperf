# List of subdirectories
SUBDIRS  = dense
SUBDIRS += sparse

# Do not edit anything below, make creates it.
# -#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#

ROOTDIR   = /home/rocco/rbench

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

PLEASE = "Do not edit anything below, make creates it."

Makefile: ${TEMPLATE}
	@echo "=*= making $@ =*="
	@cat $@ | head -`grep -n ${PLEASE} $@ | head -1 | cut -d ':' -f 1` > $@.tmp
	@echo "# -#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#" >> $@.tmp
	@echo >> $@.tmp
	@cat $^ >> $@.tmp
	@mv $@.tmp $@

tar:
	@echo -n "Making distribution/backup file trm.tar.gz ..."
	@(cd ${ROOTDIR} && make -s distclean && cd .. && tar cfz trm.tar.gz trm && mv trm.tar.gz trm/)
	@echo "Done"

# Fake targets
.PHONY: all clean distclean ${SUBDIRS}

