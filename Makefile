CXX = clang++
CXXFLAGS = -std=c++20 -stdlib=libc++ -Wall -g -Iinclude
SRC = $(wildcard src/*.cpp)
OUT = main

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(OUT)
