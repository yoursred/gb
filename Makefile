# Basic bitch makefile


# TODO: Make a good makefile
GPP=g++ 
# x86_64-apple-darwin21-g++-12

SRCDIR=src
INCL=$(SRCDIR)
FILES=$(wildcard $(SRCDIR)/*.cpp)
BUILDDIR=build
VERSION=\"0.0.1_$(shell git rev-parse --short HEAD)\"

CXXFLAGS=-lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system -I$(INCL) -g -Wall -Wextra -std=c++17 -DVERSION=$(VERSION)

.PHONY: clean

$(shell mkdir -p build/cpu build/ppu)

build/gb: build/main.o build/debugger/*.o build/cpu/*.o build/memory/*.o build/ppu/*.o
	$(GPP) $(BUILDDIR)/ppu/*.o $(BUILDDIR)/cpu/*.o $(BUILDDIR)/memory/*.o $(BUILDDIR)/debugger/*.o $(BUILDDIR)/main.o -o $(BUILDDIR)/gb $(CXXFLAGS) 

build/main.o: src/main.cpp
	$(GPP) $(CXXFLAGS) -c src/main.cpp -o $(BUILDDIR)/main.o

# build/ppu.o: build/ppu/*.o
# 	ld -r $(BUILDDIR)/ppu/*.o -o $(BUILDDIR)/ppu.o -nostdlib

build/debugger/*.o: src/debugger/*
	mkdir -p build/debugger/
	$(GPP) $(CXXFLAGS) -c src/debugger/*.cpp
	mv *.o $(BUILDDIR)/debugger

build/ppu/*.o: src/ppu/*
	mkdir -p build/ppu/
	$(GPP) $(CXXFLAGS) -c src/ppu/*.cpp
	mv *.o $(BUILDDIR)/ppu


# build/cpu.o: build/cpu/*.o
# 	ld -r $(BUILDDIR)/cpu/*.o -o $(BUILDDIR)/cpu.o -nostdlib

build/memory/*.o: src/memory/*
	mkdir -p build/memory
	$(GPP) $(CXXFLAGS) -c src/memory/*.cpp
	mv *.o $(BUILDDIR)/memory

build/cpu/*.o: src/cpu/*
	mkdir -p build/cpu
	$(GPP) $(CXXFLAGS) -c src/cpu/*.cpp
	mv *.o $(BUILDDIR)/cpu


clean:
	rm -rf build