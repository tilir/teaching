#------------------------------------------------------------------------------
#
# Lazy evaulation example. Output:
# bar = Hello World
# qux = Hello Make
#
#------------------------------------------------------------------------------

bar = Hello $(baz)
baz = World
quux = $(baz)
qux = Hello $(quux)
quux = Make

.PHONY: all
all:
	@echo "bar = $(bar)"
	@echo "qux = $(qux)"
