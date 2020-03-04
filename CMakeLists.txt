cmake_minimum_required(VERSION 3.7)

project(MildDBMS)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++17 -pthread")

add_subdirectory(thirdparty/googletest)

# Find all executables
file(GLOB project_SRCS src/*.cpp) #src/*.h


# Add project executable
add_executable(mildDBMS ${project_SRCS})


add_executable(test test/utest_mildDBMS.cpp src/FileScan.cpp src/Selection.cpp src/Projection.cpp src/schema_loader.cpp src/Join.cpp src/QueryPlanner.cpp src/Tokenizer.cpp)

target_link_libraries(test gtest_main)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
