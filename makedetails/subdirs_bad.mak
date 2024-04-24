#------------------------------------------------------------------------------
#
# Bad subdirs example.
#
#------------------------------------------------------------------------------

# neat output wiuthout "Entering/Leaving" stuff
MAKEFLAGS += --no-print-directory

# for bash this is list, so for make
SUBDIRS = sub1 sub2 sub3

.PHONY: subdirs
subdirs:
	@for dir in $(SUBDIRS); do \
	  $(MAKE) -C $$dir; \
	done
