# CPPND: Capstone Repo

This is a Capstone project for the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213).

The project implements a database management system based on specs from Oz Nova's course in databases at the Bradfield School of Computer Science in San Franisco, and Joe Hellerstein’s databases course at UC Berkeley.
Movie data from GroupLens Research is used to demonstrate the system's functionality. (https://grouplens.org/datasets/movielens/)

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo, including the test submodule, like this: git clone --recurse-submodules https://github.com/tony-landreth/CppND-Capstone-DBMS.git
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./mildDBMS`.

## Testing

The testing executable is also placed in the `build` directory. From within `build`, you can run the unit tests as follows:
```
./test
```
NOTE: The test suite requires that you've installed the testing submodule. See Basic Build Instructions.