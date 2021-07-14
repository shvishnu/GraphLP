cmake_minimum_required(VERSION 2.8)
project(getlp)

# compiler and flags
# set(CMAKE_C_COMPILER "clang")
# set(CMAKE_CXX_COMPILER "clang++")
#add_definitions("-std=c++14 -Wall -O3")
add_definitions("-std=c++14 -Wall -O0 -g")
# -fno-limit-debug-info")

# include directory and the libraries we will build
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_subdirectory(lib)

# build and install main
add_executable(getlp main.cc "include/BipartiteGraph.h" "include/GraphReader.h")
target_link_libraries(getlp graph)
install(TARGETS getlp RUNTIME DESTINATION bin)

add_executable(rev rev.cc "include/BipartiteGraph.h" "include/GraphReader.h")
target_link_libraries(rev graph)
install(TARGETS rev RUNTIME DESTINATION bin)
