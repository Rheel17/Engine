cmake_minimum_required(VERSION 3.13.2)

# Build options
option(BUILD_SANDBOX "Build the Sandbox sample project" ON)
set(EXTERNAL_INCLUDE_DIRECTORY "" CACHE STRING "Include directory for external libraries")
set(EXTERNAL_LIBRARY_DIRECTORY "" CACHE STRING "Library directory for external libraries")

# Set project name
project(Engine)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Build resources
set(RESOURCE_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/_res.h" "${CMAKE_CURRENT_BINARY_DIR}/_res.cpp")
add_custom_target(Resources
        COMMAND python build_res.py ${RESOURCE_OUTPUT}
        WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}
        COMMENT "Building Resources")

# Create the library
add_library(RheelEngine SHARED)
target_compile_definitions(RheelEngine PRIVATE BUILDING_RHEELENGINE_DLL)

# Copy shared libraries
add_custom_command(
        TARGET RheelEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/lib ${CMAKE_CURRENT_BINARY_DIR})

# Add resources to the library
add_dependencies(RheelEngine Resources)
set_source_files_properties(${RESOURCE_OUTPUT} PROPERTIES GENERATED 1)
target_sources(RheelEngine PRIVATE ${RESOURCE_OUTPUT})
target_include_directories(RheelEngine PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# Add sources to the library
target_sources(RheelEngine PUBLIC src/RheelEngine.h PRIVATE src/RheelEngine/_common.h)
add_subdirectory(src)

# Add external libraries
target_include_directories(RheelEngine PUBLIC ${EXTERNAL_INCLUDE_DIRECTORY})
target_link_directories(RheelEngine PUBLIC ${EXTERNAL_LIBRARY_DIRECTORY})

target_link_libraries(RheelEngine PRIVATE glfw3)
target_link_libraries(RheelEngine PRIVATE gdi32)
target_link_libraries(RheelEngine PRIVATE opengl32)
target_link_libraries(RheelEngine PRIVATE glew32)
target_link_libraries(RheelEngine PRIVATE png16)
target_link_libraries(RheelEngine PRIVATE freetype)
target_link_libraries(RheelEngine PRIVATE vhacd)
target_link_libraries(RheelEngine PRIVATE alut)
target_link_libraries(RheelEngine PUBLIC BulletDynamics)
target_link_libraries(RheelEngine PUBLIC BulletCollision)
target_link_libraries(RheelEngine PUBLIC LinearMath)
target_link_libraries(RheelEngine PRIVATE libOpenAL32.dll.a)

# Build the sandbox, if required.
if (BUILD_SANDBOX)
    include(sandbox/CMakeLists.txt)
endif()
