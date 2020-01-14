# RheelEngine
*A work in progress game engine written in C++/OpenGL*

## Overview
This project contains the source for the engine in <code>/src</code>, as 
well as a sample game project in <code>/sandbox</code>. 

## Dependencies
This project uses the following external dependencies:
* [freealut 1.10](https://github.com/vancegroup/freealut)
* [Bullet3 2.88](https://github.com/bulletphysics/bullet3)
* [FreeType](https://www.freetype.org/)
* [GLEW 2.1.0](http://glew.sourceforge.net/)
* [GLFW 3.3](https://www.glfw.org/)
* [libpng 1.6.36](http://www.libpng.org/pub/png/libpng.html)
* [vhacd](https://github.com/kmammou/v-hacd)

## Build
To build, CMake and Python are needed. Tested to work with MinGW.

1. Copy external shared libraries (Windows: .dll files) to the `lib` folder:
    * glew32
    * libalut
    * libpng16
    * libzlib
    * OpenAL32
2. Create a build directory:
	* `mkdir build`
	* `cd build`
3. Run CMake:
	* `cmake .. [-G <generator>] [options]`
		
		Use the `-G <generator>` to specify the 
	    [generator](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html)
	    for CMake.
	
	    Use the options to specify the location of the external libraries. If the
	    libraries are located in the compiler search path, this is not necessary.
	
        * Libary includes: `-DEXTERNAL_INCLUDE_DIRECTORY="<directory>"`
        * Libary libs: `-DEXTERNAL_LIBRARY_DIRECTORY="<directory>"`

4. Run your build tool.
    
    For example, with MinGW:
    
    * `make`

<sup><sub>© 2020 Levi van Rheenen</sub></sup>



