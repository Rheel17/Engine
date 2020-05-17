# RheelEngine
*A work in progress game engine written in C++/OpenGL*

## Overview
This project contains the source for the engine in <code>/src</code>, as 
well as a sample game project in <code>/sandbox</code>. 

## Dependencies
This project uses the following external dependencies:
* [GLM](https://glm.g-truc.net/)
* [GLFW 3.3.2](https://www.glfw.org/)
* [GLEW 2.1.0](http://glew.sourceforge.net/)
* [RapidXml](http://rapidxml.sourceforge.net/)
* [freealut 1.10](https://github.com/vancegroup/freealut) (windows) [alut](https://svn.ict.usc.edu/svn_vh_public/trunk/lib/vhcl/openal/docs/alut.html) (linux)
* [FreeType](https://www.freetype.org/)
* [Bullet3 2.89](https://github.com/bulletphysics/bullet3)
* [libpng 1.6.37](http://www.libpng.org/pub/png/libpng.html)

## Build
To build, CMake and Python 3.x are needed. Tested to work with MinGW.

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



