# Justfile for culminate C++ library

# Default task - list all available tasks
default:
    @just --list

# Build the project
build type:
    cd build && cmake -DCMAKE_BUILD_TYPE={{type}} .. && make -j4

# Build Debug (default)
build-debug:
    cd build && cmake -DCMAKE_BUILD_TYPE=Debug .. && make -j4

# Build Release
build-release:
    cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && make -j4

# Run the example
example:
    cd build && ./examples/example

# Run all tests
tests:
    cd build && ctest --output-on-failure

# Run benchmarks
benchmark:
    cd build && ./tests/test_benchmark

# Clean build directory
clean:
    rm -rf build

# Reconfigure and build from scratch
rebuild type:
    rm -rf build && mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE={{type}} .. && make -j4
