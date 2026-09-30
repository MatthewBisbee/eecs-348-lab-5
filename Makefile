CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O2

.PHONY: all clean test

all: main

main: main.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

test: all
	python3 tests/test_matrix.py

clean:
	$(RM) main
