CXX := g++
CXXFLAGS := -Wall -Wextra -Iinc -ggdb -std=c++20

SRC := $(wildcard src/*.cpp)
OBJ := $(patsubst src/%.cpp, build/%.o, $(SRC))

build/main: $(OBJ)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -o $@ $^

build/%.o: src/%.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c -o $@ $<

run: ./build/main
	$^ Makefile

clean:
	rm -rf build

.PHONY: build run clean
