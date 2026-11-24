# Memory Stride Benchmark

C++ microbenchmark measuring how memory access stride affects effective memory bandwidth across CPU cache levels and main memory.

## Overview
- **Working set sizes:** 32 KB to 131,072 KB (128 MB)
- **Strides:** 8 bytes to 4,096 bytes
- **Access pattern:** Sequential/strided read-accumulate loop

## Build & Run (Visual Studio)
1. Open the solution/project file in **Visual Studio** (with the Desktop development with C++ workload).
2. Switch the build configuration in the top toolbar from `Debug` to **`Release`** (`x64`).
3. Build the solution (`Ctrl + Shift + B`) and run without debugging (`Ctrl + F5`).

## Command-Line Compilation (MSVC / g++)
- **MSVC (Developer Command Prompt):**
  ```cmd
  cl /O2 /EHsc MemoryBenchmark.cpp /Fe:memory_benchmark.exe
  memory_benchmark.exe
  ```
- **g++ / Clang:**
  ```bash
  g++ -O3 -std=c++17 MemoryBenchmark.cpp -o memory_benchmark
  ./memory_benchmark
  ```
