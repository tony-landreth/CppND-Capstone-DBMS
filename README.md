# MildDBMS Repo

This is a hobby project, implementing a database management system very loosely based on Joe Hellerstein’s databases course at UC Berkeley. In fact, don't blame Hellerstein for any of this.

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
