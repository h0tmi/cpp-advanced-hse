CXX = clang++
CXXFLAGS = -std=c++17 -Wall

# List all object files needed for the final binary.
objects = deep_thought.o thinker.o

all: $(objects) link run

# Implicit rules for C/C++ files:
#   `n.o` is made automatically from `n.cc` or `n.cpp`
#   with a command of the form $(CXX) -c $(CPPFLAGS) $(CXXFLAGS)
deep_thought.o: deep_thought.cpp
thinker.o: thinker.cpp

# Or like this:
# deep_thought.o thinker.o: deep_thought.cpp thinker.cpp

# Even shorter:
# $(objects): %.o: %.cpp

link: deep_thought.o thinker.o
	$(CXX) deep_thought.o thinker.o -o thinker

run: link
	./thinker

clean:
	rm -rf *.o thinker
