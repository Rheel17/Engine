cmake_minimum_required(VERSION 3.13.2)

# Build options
option(BUILD_SANDBOX "Build the Sandbox sample project" ON)
option(BUILD_TEST "Build the googletest unit test" OFF)

# Use C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
#set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic")

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

# Packages
find_package(Python3 COMPONENTS Interpreter)
find_package(OpenGL REQUIRED)
find_package(OpenAL REQUIRED)
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

# Generate internal component ids
add_custom_target(ComponentIdGen
		COMMAND python3 auto_component_id.py
		WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}
		COMMENT "Generating Component Ids")

# Create the library
add_library(RheelEngine STATIC)
target_compile_definitions(RheelEngine PRIVATE RE_DEBUG)

# Add resources to the library
add_dependencies(RheelEngine Resources)
set_source_files_properties(${RESOURCE_OUTPUT} PROPERTIES GENERATED 1)
target_sources(RheelEngine PRIVATE ${RESOURCE_OUTPUT})
target_include_directories(RheelEngine PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# Add dependency to the header and component id generator
add_dependencies(RheelEngine Header ComponentIdGen)

# Add sources to the library
target_sources(RheelEngine PRIVATE src/RheelEngine.h src/RheelEngine/_common.h)
add_subdirectory(src)

# Add external libraries
target_include_directories(RheelEngine PUBLIC ${FREETYPE_INCLUDE_DIRS})
target_include_directories(RheelEngine PUBLIC ${BULLET_INCLUDE_DIRS})

set_target_properties(RheelEngine PROPERTIES LINK_SEARCH_START_STATIC 1)
set_target_properties(RheelEngine PROPERTIES LINK_SEARCH_END_STATIC 1)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")

target_link_libraries(RheelEngine PUBLIC glfw3 glew32 opengl32)
target_link_libraries(RheelEngine PUBLIC ${OPENAL_LIBRARY})
target_link_libraries(RheelEngine PUBLIC freetype brotlicommon-static freetype bz2 freetype harfbuzz freetype brotlidec-static freetype graphite2 pcre iconv glib-2.0 intl Rpcrt4)
target_link_libraries(RheelEngine PUBLIC ${BULLET_LIBRARIES})
target_link_libraries(RheelEngine PUBLIC png z)
target_link_libraries(RheelEngine PUBLIC alut)
target_link_libraries(RheelEngine PUBLIC pugixml)

# Installation
include(GNUInstallDirs)

install(TARGETS RheelEngine
		EXPORT RheelEngineConfig
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY src/
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
		FILES_MATCHING PATTERN "*.h")

install(EXPORT RheelEngineConfig DESTINATION share/RheelEngine/cmake)
export(TARGETS RheelEngine FILE RheelEngineConfig.cmake EXPORT_LINK_INTERFACE_LIBRARIES)

add_subdirectory(wowie3)
