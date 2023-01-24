# Optimized Project 2

This project has optimized speed over the original project 2 implementation.

The algorithm we used here is naive convolution.

## How to run

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j8
./face_detection_demo [-s] [-h] [-d <dir>]
```

Also, a shared library is provided, which can be linked to your own project.

```cmake
target_link_libraries(<your_target> [Private|Public] face_detection::lib)
```
