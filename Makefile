CXX=clang++
CXXFLAGS=-O3 --std=c++14 -I./toolbox-cpp/include -I./include
LDFLAGS=
WIN_LDFLAGS=$(LDFLAGS) -static-libgcc -static-libstdc++ -static -lpthread -L .
LDLIBS=
RC_DIR=./resources
OUTPUT=randgrid

all: $(OUTPUT)

randgrid: randgrid.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $< -o $@

randgrid.exe: randgrid.cpp
	x86_64-w64-mingw32-g++ $(WIN_LDFLAGS) $(CXXFLAGS) $< -o $@

clean:
	rm -r $(OUTPUT) randgrid.exe

