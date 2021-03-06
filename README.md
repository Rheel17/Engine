# RheelEngine
*A work in progress game engine written in C++/OpenGL*

## Overview
This project contains the source for the engine in <code>/src</code>, as 
well as a sample game project in <code>/sandbox</code>. 

## Dependencies
This project uses the following external dependencies:
* [GLWR](https://github.com/Rheel17/glwr)
* [GLEW](http://glew.sourceforge.net/)
* [GLFW 3](https://www.glfw.org/)
* [GLM](https://glm.g-truc.net/)
* [FreeType](https://www.freetype.org/)
* [Bullet](https://github.com/bulletphysics/bullet3)
* [libpng](http://www.libpng.org/pub/png/libpng.html)
* [pugixml](https://github.com/zeux/pugixml)
* [freealut](https://github.com/vancegroup/freealut) (Windows) [alut](https://svn.ict.usc.edu/svn_vh_public/trunk/lib/vhcl/openal/docs/alut.html) (Linux)

## Build
To build, CMake and Python 3.x are needed. Tested to work with MinGW (Windows) and GCC 10.1 (Linux).

1. Create a build directory:
	* `mkdir build`
	* `cd build`
2. Run CMake:
	* `cmake .. [-G <generator>] [options]`
		
		Use the `-G <generator>` to specify the 
	    [generator](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html)
	    for CMake.
	
	    Use the options to specify the location of the external libraries. If the
	    libraries are located in the compiler search path, this is not necessary.
	
        * Libary includes: `-DEXTERNAL_INCLUDE_DIRECTORY="<directory>"`
        * Libary libs: `-DEXTERNAL_LIBRARY_DIRECTORY="<directory>"`

3. Run your build tool.
    
    * `cmake --build .` should work for most platforms
    * `make -j <cores>` for linux/MinGW for multithreaded building

<sup><sub>© 2020 Levi van Rheenen</sub></sup>



