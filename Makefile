# Basic bitch makefile

GPP=g++

SRCDIR=src
INCL=$(SRCDIR)
FILES=$(wildcard $(SRCDIR)/*.cpp)
BUILDDIR=build

.PHONY: clean

build/gb: build/*.o
	$(GPP) -o build/gb build/*.o

build/*.o: src/*.cpp
	$(GPP) -c $(FILES) -I$(INCL)
	mv *.o $(BUILDDIR)

clean:
	rm -f build/*