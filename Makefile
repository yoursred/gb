# Basic bitch makefile

GPP=g++

SRCDIR=src
INCL=$(SRCDIR)
FILES=$(wildcard $(SRCDIR)/*.cpp)
BUILDDIR=build

.PHONY: clean

build/gb: build/*.o
	$(GPP) -g -O3 -o build/gb build/*.o
	rm build/*.o

build/*.o: src/*.cpp
	$(GPP) -g -O3 -c $(FILES) -I$(INCL)
	mv *.o $(BUILDDIR)

clean:
	rm -f build/*