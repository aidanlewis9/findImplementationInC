CC=		gcc
CFLAGS=		-g -gdwarf-2 -Wall -std=gnu99
LD=		gcc
LDFLAGS=	-L.
AR=		ar
ARFLAGS=	rcs
TARGETS=	search

all:		$(TARGETS)

execute.o:	execute.c search.h
	@echo Compiling execute.o...
	@$(CC) $(CFLAGS) -c -o $@ $<

filter.o:	filter.c search.h
	@echo Compiling filter.o...
	@$(CC) $(CFLAGS) -c -o $@ $<

main.o:		main.c search.h
	@echo Compiling main.o...
	@$(CC) $(CFLAGS) -c -o $@ $<

search.o:	search.c search.h
	@echo Compiling search.o...
	@$(CC) $(CFLAGS) -c -o $@ $<

utilities.o:	utilities.c search.h
	@echo Compiling utilities.o...
	@$(CC) $(CFLAGS) -c -o $@ $<

search:		execute.o filter.o main.o search.o utilities.o
	@echo Linking search...
	@$(LD) $(LDFLAGS) -o $@ $^

test:		search test_search.sh
	@echo Testing $<...
	@./test_search.sh

clean:
	@echo Cleaning...
	@rm -f $(TARGETS) *.o *.log *.input

.PHONY:		all test benchmark clean

# TODO: Add rules for search and object files
