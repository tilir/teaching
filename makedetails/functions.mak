#------------------------------------------------------------------------------
#
# Patsubst example (and general functions usage)
#
#------------------------------------------------------------------------------

cfiles = main1.c foo.c main2.c bar.c
mains = main1.c main2.c
filtered = $(filter-out $(mains),$(cfiles))
objs = $(patsubst %.c, %.o, $(cfiles))
ofilt = $(filtered:%.c=%.o)

.PHONY: all
all:
	@echo "cfiles = $(cfiles)"
	@echo "mains = $(mains)"
	@echo "filtered = $(filtered)"
	@echo "objs = $(objs)"
	@echo "ofilt = $(ofilt)"

