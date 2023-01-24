# Optimized Project 2

This project has optimized speed over the original project 2 implementation.

The algorithm we used here is naive convolution.

***Note that first run of the demo might be slower on linux machine due to serveral factors like lazy linking (see [this answer](https://stackoverflow.com/questions/54781218/execution-time-of-c-code-on-linux-for-the-first-time-is-extremly-slow)), try to run the demo executable once before benchmarking***

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
