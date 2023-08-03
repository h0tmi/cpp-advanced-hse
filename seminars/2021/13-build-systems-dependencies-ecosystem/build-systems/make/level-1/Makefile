# Hello world in Make.
# This is the first target which makes it a default one for the `make` command.
hello:
	echo "Hello, world!"

# Variables are simple key-value mappings.
# Type of a value is always `string`.
CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Werror -Wextra -Wpedantic

# General syntax:
#
# targets: prerequisites
# \t command
# \t command
# \t command
#
# Tabs are important -- they are part of the syntax.

run_thinker: thinker
	./thinker

thinker: thinker.o deep_thought.o
	$(CXX) thinker.o deep_thought.o -o thinker

thinker.o: thinker.cpp
	$(CXX) $(CXXFLAGS) -c thinker.cpp -o thinker.o

deep_thought.o: deep_thought.cpp
	$(CXX) $(CXXFLAGS) -c deep_thought.cpp -o deep_thought.o

# Naming `clean` is a convention, but not an enforced one.
clean:
	rm -f *.o thinker
