# method
Experimental C++11 OpenGL 3D rendering engine. May be extended in the future 
to have more features specific to game programming. Note that the engine is
only Linux compatible at the moment.

![The Stanford Dragon](https://raw.githubusercontent.com/baszy/method/master/screenshots/dragon.png)

## Features
- Blinn-Phong shading model + normal, specular mapping
- Uncompressed TGA texture loading
- OBJ model loading with shared-vertex optimization for OpenGL

## Building
Move to the desired build directory (in-source builds are not recommended) and
invoke CMake:
```
mkdir build
cd build
cmake ./path/to/CMakeLists.txt
```
The main library and test program can then be built by running `make`.
