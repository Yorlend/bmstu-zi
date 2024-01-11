CXX := g++
CXXFLAGS := -Wall -Wextra -Iinc -ggdb

SRC := $(wildcard src/*.cpp)
OBJ := $(patsubst src/%.cpp, build/%.o, $(SRC))

build/main: $(OBJ)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -o $@ $^

build/%.o: src/%.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c -o $@ $<

run: ./build/main
	$^ 1234567890abc -e secret_message.txt secret_message.enc
	$^ 1234567890abc -d secret_message.enc secret_message_dec.txt
	diff secret_message.txt secret_message_dec.txt

	$^ 1234567890abc -e cat.png cat.png.enc
	$^ 1234567890abc -d cat.png.enc cat.png_dec.png
	diff cat.png cat.png_dec.png

clean:
	rm -rf build secret_message_dec.txt secret_message.enc cat.png.enc cat.png_dec.png

.PHONY: build run clean
