# CPPND: Capstone Repo

This is a Capstone project for the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213).

The project implements a database management system very loosely based on Joe Hellerstein’s databases course at UC Berkeley.
In fact, don't blame Hellerstein for any of this.

Movie data from GroupLens Research is used to demonstrate the system's functionality. (https://grouplens.org/datasets/movielens/)

# Project Structure  

- When a user issues a query, the QueryPlanner uses a Tokenizer to parse the query.  
- The Tokenizer stores tokens in a TokenTree.  
- Based on the contents of the TokenTree, the QueryPlanner builds a pipeline of PlanNodes.  
- Each of the major SQL operators modeled in this project have their own PlanNode subclass.  - For example, `SELECT * FROM test_data WHERE title EQUALS 'The Fall';` causes the QueryPlanner
  to build a pipeline consisting of a `FileScan -> Selection -> Projection`. Different queries require different pipelines.  

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

## Rubric Points Addressed  
This project doesn't use templates or concurrency features, unfortunately. Here are the
rubric items it does use:  
- [ ] A variety of control structures are used in the project.   
- [ ] The project code is clearly organized into functions.  
- [ ] The project reads data from an external file or writes data to a file as part of the necessary operation of the program. (see `FileScan`)   
- [ ] The project accepts input from a user as part of the necessary operation of the program. (see `main.cpp`)  
- [ ] The project code is organized into classes with class attributes to hold the data, and class methods to perform tasks. (see any `PlanNode` class)
- [ ] All class data members are explicitly specified as public, protected, or private. (see any `PlanNode` class)  
- [ ] All class members that are set to argument values are initialized through member initialization lists.
- [ ] All class member functions document their effects, either through function names, comments, or formal documentation. Member functions do not change program state in undocumented ways.
- [ ] Appropriate data and functions are grouped into classes. Member data that is subject to an invariant is hidden from the user. State is accessed via member functions.  
- [ ] Inheritance hierarchies are logical. Composition is used instead of inheritance when appropriate. Abstract classes are composed of pure virtual functions. Override functions are specified.  
- [ ] One function is overloaded with different signatures for the same function name. (see `TokenTree.h` constructor)
- [ ] One member function in an inherited class overrides a virtual base class member function.  
- [ ] At least two variables are defined as references, or two functions use pass-by-reference in the project code. (see `TokenTree::find` or any `PlanNode` constructor)  
- [ ] For all classes, if any one of the copy constructor, copy assignment operator, move constructor, move assignment operator, and destructor are defined, then all of these functions are defined. (see `TokenTree.h`)
- [ ] The project uses at least one smart pointer: unique_ptr, shared_ptr, or weak_ptr. The project does not use raw pointers. (see `QueryPlanner.cpp`)  




## Basic Build Instructions

1. Clone this repo, including the test submodule, like this: git clone --recurse-submodules https://github.com/tony-landreth/CppND-Capstone-DBMS.git
2. Make a build directory in the top level directory: `mkdir build && cd build`  
3. Compile: `cmake .. && make`

## Running It
This is a read-only DBMS with very limited capabilities. Keep that in mind.
Assuming that you're in the same directory as the executable (probably `build/`). The following lines will run:
1. `./mildDBMS "SELECT * FROM movies;"`  
2. `./mildDBMS "SELECT movies.title, ratings.rating FROM movies JOIN ratings ON movieId = movieId;"`  
3. `./mildDBMS "SELECT movies.title, ratings.rating FROM movies JOIN ratings ON movieId = movieId WHERE title EQUALS 'Sudden Death (1995)';"`  
4. `./mildDBMS "SELECT movies.title FROM movies JOIN movies ON movieId = movieId WHERE title EQUALS 'Sudden Death (1995)';"`  

## Testing

The testing executable is also placed in the `build` directory. From within `build`, you can run the unit tests as follows:
```
./test
```
NOTE: The test suite requires that you've installed the testing submodule. See Basic Build Instructions.
