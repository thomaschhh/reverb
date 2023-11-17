# Reverb


## CMake

Install [CMake](https://cmake.org/).
In order to generate your project with CMake, follow these steps:

```
cd /path/to/Reverb
cmake -B build -G XCode     # to generate and build with a XCode project
cmake --build build
```
or
```
cd /path/to/Reverb
cmake -B build             # to generate with the default generator
cmake --build build
```