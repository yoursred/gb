# Basic bitch makefile

GPP=g++ 
# x86_64-apple-darwin21-g++-12

SRCDIR=src
INCL=$(SRCDIR)
FILES=$(wildcard $(SRCDIR)/*.cpp)
BUILDDIR=build

CXXFLAGS=-I$(INCL) -g -Wall -Wextra -std=c++11

.PHONY: clean

$(shell mkdir -p build/cpu build/ppu)

build/gb: build/main.o build/cpu/*.o build/ppu/*.o
	$(GPP) $(CXXFLAGS) $(BUILDDIR)/cpu/*.o $(BUILDDIR)/main.o  -o $(BUILDDIR)/gb

build/main.o: src/main.cpp
	$(GPP) $(CXXFLAGS) -c src/main.cpp -o $(BUILDDIR)/main.o

# build/ppu.o: build/ppu/*.o
# 	ld -r $(BUILDDIR)/ppu/*.o -o $(BUILDDIR)/ppu.o -nostdlib

build/ppu/*.o: src/ppu/*
	mkdir -p build/ppu/
	$(GPP) $(CXXFLAGS) -c src/ppu/*.cpp
	mv *.o $(BUILDDIR)/ppu


# build/cpu.o: build/cpu/*.o
# 	ld -r $(BUILDDIR)/cpu/*.o -o $(BUILDDIR)/cpu.o -nostdlib

build/cpu/*.o: src/cpu/*
	mkdir -p build/cpu
	$(GPP) $(CXXFLAGS) -c src/cpu/*.cpp
	mv *.o $(BUILDDIR)/cpu


clean:
	rm -rf build