cmake_minimum_required(VERSION 3.13.2)

# Build options
option(BUILD_SANDBOX "Build the Sandbox sample project" ON)
option(BUILD_TEST "Build the googletest unit test" ON)
set(EXTERNAL_INCLUDE_DIRECTORY "" CACHE STRING "Include directory for external libraries")
set(EXTERNAL_LIBRARY_DIRECTORY "" CACHE STRING "Library directory for external libraries")

# Build the sandbox, if required.
if (BUILD_SANDBOX)
	include(sandbox/CMakeLists.txt)
endif()

# Add the test, if required
if (BUILD_TEST)
	include(test/CMakeLists.txt)
endif()

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

# Generate RheelEngine.h header
set(RHEELENGINE_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/src/RheelEngine.h")
add_custom_target(Header
		COMMAND python include_headers_gen.py
		WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}
		COMMENT "Generating RheelEngine.h")

# Create the library
add_library(RheelEngine SHARED)
target_compile_definitions(RheelEngine PRIVATE BUILDING_RHEELENGINE_DLL RE_DEBUG)

# Copy shared libraries
add_custom_command(
		TARGET RheelEngine POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/lib ${CMAKE_CURRENT_BINARY_DIR})

# Add resources to the library
add_dependencies(RheelEngine Resources)
set_source_files_properties(${RESOURCE_OUTPUT} PROPERTIES GENERATED 1)
target_sources(RheelEngine PRIVATE ${RESOURCE_OUTPUT})
target_include_directories(RheelEngine PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# Add dependency to the header generator
add_dependencies(RheelEngine Header)

# Add sources to the library
target_sources(RheelEngine PUBLIC src/RheelEngine.h PRIVATE src/RheelEngine/_common.h)
add_subdirectory(src)

# Add external libraries
target_include_directories(RheelEngine PUBLIC ${EXTERNAL_INCLUDE_DIRECTORY})
target_link_directories(RheelEngine PUBLIC ${EXTERNAL_LIBRARY_DIRECTORY})

if(WIN32)
	target_link_libraries(RheelEngine PRIVATE glfw3)
	target_link_libraries(RheelEngine PRIVATE gdi32)
	target_link_libraries(RheelEngine PUBLIC glew)
	target_link_libraries(RheelEngine PUBLIC opengl32)
else()
	target_link_libraries(RheelEngine PRIVATE glfw)
	target_link_libraries(RheelEngine PUBLIC GLEW)
	target_link_libraries(RheelEngine PUBLIC GL)
endif(WIN32)

target_link_libraries(RheelEngine PRIVATE png16)

if (WIN32)
	target_link_libraries(RheelEngine PRIVATE zlibstatic)
else()
	target_link_libraries(RheelEngine PRIVATE z)
endif()

target_link_libraries(RheelEngine PRIVATE freetype)
target_link_libraries(RheelEngine PUBLIC BulletDynamics)
target_link_libraries(RheelEngine PUBLIC BulletCollision)
target_link_libraries(RheelEngine PUBLIC LinearMath)

if (WIN32)
	target_link_libraries(RheelEngine PRIVATE libOpenAL32.dll.a)
else()
	target_link_libraries(RheelEngine PRIVATE openal)
endif()

target_link_libraries(RheelEngine PRIVATE alut)
target_compile_definitions(RheelEngine PUBLIC GLEW_STATIC)

