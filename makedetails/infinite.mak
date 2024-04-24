#------------------------------------------------------------------------------
#
# Lazy evaulation example. Output:
# Recursive variable 'bar' references itself (eventually)
#
#------------------------------------------------------------------------------

bar = Hello $(bar)

.PHONY: all
all:
	@echo "bar = $(bar)"
