#------------------------------------------------------------------------------
#
# Lazy evaulation example. Output:
# bar = Hello
# qux = Hello World
# corge = Hello
#
#------------------------------------------------------------------------------

bar := Hello $(baz)
baz := World
quux := $(baz)
qux := Hello $(quux)
quux := Make
corge := Hello $(corge)

.PHONY: all
all:
	@echo "bar = $(bar)"
	@echo "qux = $(qux)"
	@echo "corge = $(corge)"
