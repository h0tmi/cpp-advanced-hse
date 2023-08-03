# Another convention: `all` == make multiple targets and run all of them.
all: create_build_directory build run

create_build_directory:
	mkdir -p build

build: create_build_directory deep_thought.cpp thinker.cpp
	clang++ -std=c++17 deep_thought.cpp thinker.cpp -o build/main

run: build
	build/main

clean:
	rm -rf build
