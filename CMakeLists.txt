cmake_minimum_required(VERSION 3.13.2)

# Build options
option(BUILD_SANDBOX "Build the Sandbox sample project" ON)
option(BUILD_TEST "Build the googletest unit test" OFF)

# Build the sandbox, if required.
if (BUILD_SANDBOX)
	add_subdirectory(sandbox)
endif()

# Add the test, if required
if (BUILD_TEST)
	add_subdirectory(test)
endif()

# Set project name
project(Engine)

# Use C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Packages
find_package(Python3 COMPONENTS Interpreter)
find_package(OpenGL REQUIRED)
find_package(OpenAL REQUIRED)
find_package(glwr REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLM REQUIRED)
find_package(Freetype REQUIRED)
find_package(Bullet REQUIRED)
find_package(PNG REQUIRED)
find_package(PUGIXML REQUIRED)

# Build resources
set(RESOURCE_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/_res.h" "${CMAKE_CURRENT_BINARY_DIR}/_res.cpp")
add_custom_target(Resources
		COMMAND python3 build_res.py ${RESOURCE_OUTPUT}
		WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}
		COMMENT "Building Resources")

# Generate RheelEngine.h header
set(RHEELENGINE_HEADER "src/RheelEngine.h")
add_custom_target(Header
		COMMAND python3 include_headers_gen.py
		WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}
		COMMENT "Generating RheelEngine.h")

# Create the library
add_library(RheelEngine SHARED)
target_compile_definitions(RheelEngine PRIVATE BUILDING_RHEELENGINE_DLL RE_DEBUG)

# Add resources to the library
add_dependencies(RheelEngine Resources)
set_source_files_properties(${RESOURCE_OUTPUT} PROPERTIES GENERATED 1)
target_sources(RheelEngine PRIVATE ${RESOURCE_OUTPUT})
target_include_directories(RheelEngine PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# Add dependency to the header generator
add_dependencies(RheelEngine Header)

# Add sources to the library
target_sources(RheelEngine PRIVATE src/RheelEngine.h src/RheelEngine/_common.h)
add_subdirectory(src)

# Add external libraries
target_include_directories(RheelEngine PUBLIC ${FREETYPE_INCLUDE_DIRS})
target_include_directories(RheelEngine PUBLIC ${BULLET_INCLUDE_DIRS})

target_link_libraries(RheelEngine PUBLIC ${OPENGL_LIBRARIES})
target_link_libraries(RheelEngine PUBLIC ${OPENAL_LIBRARY})
target_link_libraries(RheelEngine PUBLIC ${FREETYPE_LIBRARIES})
target_link_libraries(RheelEngine PUBLIC ${BULLET_LIBRARIES})
target_link_libraries(RheelEngine PUBLIC ${PNG_LIBRARY})
target_link_libraries(RheelEngine PUBLIC glew32 glfw3 pugixml alut)

# Installation
include(GNUInstallDirs)

install(TARGETS RheelEngine
		EXPORT RheelEngineConfig
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY src/
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
		FILES_MATCHING PATTERN "*.h")

install(EXPORT RheelEngineConfig DESTINATION share/RheelEngine/cmake)
export(TARGETS RheelEngine FILE RheelEngineConfig.cmake)
