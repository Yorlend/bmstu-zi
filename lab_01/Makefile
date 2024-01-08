CXX := g++
CXXFLAGS := -Wall -Wextra -Werror -Iinc -ggdb

SRC := $(wildcard src/*.cpp)
OBJ := $(patsubst src/%.cpp, build/%.o, $(SRC))

build/main: $(OBJ)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -o $@ $^

build/%.o: src/%.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c -o $@ $<

run: ./build/main
	@$^ secret_message.txt secret_message.enc
	@$^ secret_message.enc secret_message_decrypted.txt

clean:
	rm -rf build

.PHONY: build run clean
