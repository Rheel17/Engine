cmake_minimum_required(VERSION 3.13.2)

# Set project name
project(Sandbox)

# Use C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Create the executable
add_executable(Sandbox)
target_sources(Sandbox PRIVATE
        main.cpp)
target_include_directories(Sandbox PRIVATE ../src)
target_link_directories(Sandbox PRIVATE RheelEngine)
target_link_libraries(Sandbox RheelEngine)

set_target_properties(Sandbox PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../)

# Use RheelEngine
add_dependencies(Sandbox RheelEngine)
