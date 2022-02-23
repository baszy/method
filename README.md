# method
Experimental OpenGL 3D rendering engine. Only Linux-compatible at the moment.

![Dabrovic Sponza](https://github.com/baszy/method/raw/master/screenshots/sponza.png)

## Features
- Blinn-Phong shading model + normal, specular mapping
- Uncompressed TGA texture loading
- OBJ model loading with smooth groups / shared-vertex optimization for OpenGL

## Building
Move to the desired build directory and invoke CMake (in-source builds are not
recommended):
```
mkdir build
cd build
cmake ./path/to/CMakeLists.txt
```
The main library and test program can then be built by running `make`.

## Running the example program
```
./build/method_example
```
Use W, A, S, D, Shift, Space to fly around the scene. The angle of the sun can
be changed with the scrollwheel.
