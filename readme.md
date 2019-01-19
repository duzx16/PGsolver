# PowerGrid Solver
This is the project for the VLSI course by Zhengxiao Du.
## Dependencies
* OpenMP
* Eigen
* CMake >= 3.13

## Environment
Tested on MacOS Mojave 10.14.2, with Apple LLVM version 10.0.0.

## Build
For MacOS:
```bash
mkdir build && cd build
cmake -DUSE_OPENMP=TRUE -DOpenMP_C_FLAGS="-Xpreprocessor -fopenmp -I/usr/local/opt/libomp/include" -DOpenMP_CXX_FLAGS="-Xpreprocessor -fopenmp -I/usr/local/opt/libomp/include" -DOpenMP_C_LIB_NAMES="omp" -DOpenMP_CXX_LIB_NAMES="omp" -DOpenMP_omp_LIBRARY=/usr/local/opt/libomp/lib/libomp.a ..
make
```
之所以会出现这么多参数是因为CMake在MacOS下的FindOpenMP功能是坏的，所以不得不手动设置。

For Linux:
```bash
mkdir build && cd build
cmake -DUSE_OPENMP=TRUE ..
make && cd src
```

或者不带OpenMP编译
```bash
mkdir build && cd build
cmake -DUSE_OPENMP=FALSE ..
make && cd src
```

## Execute
```bash
./PGSolver source_file output_file
```
