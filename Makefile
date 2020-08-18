CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-Werror -Wall -Wextra -Wconversion -std=c++17 -O3 -g -I dep/catch2

%.o: src/%.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c $<

%: %.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

all: walk_generator

walk_generator: walk_generator.o

graph_test: graph_test.o

walk_generator.o: src/walk_generator.cpp

clean:
	$(RM) walk_generator.o

distclean: clean
	$(RM) walk_generator
