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
	@echo Тест на текстовом файле:
	@echo Сжимаем....
	@$^ compress atom.md atom_szhatiy.md
	@echo Разжимаем....
	@$^ decompress atom_szhatiy.md atom_raszhatiy.md
	@echo Тест на бинарном файле:
	@echo Сжимаем....
	@$^ compress lizun.gif lizun.szhatiy
	@echo Разжимаем....
	@$^ decompress lizun.szhatiy lizun_raszhatiy.gif

clean:
	rm -rf build *szhatiy* *raszhatiy*

.PHONY: build run clean
