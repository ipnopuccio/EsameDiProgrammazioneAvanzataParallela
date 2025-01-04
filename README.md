# Simple Ray Tracer

A parallel ray tracer implementation in C that renders 3D scenes containing spheres into PPM images. The program uses OpenMP for parallel processing and memory mapping for efficient image handling.

## Features

- Parallel ray tracing using OpenMP
- Memory-mapped image handling for efficient I/O
- Support for scenes with multiple spheres
- PPM (P6) image output format
- Custom scene definition format
- Performance timing measurements

## Prerequisites

The program requires the following components to be installed:

- LLVM/Clang compiler
- OpenMP library
- Make build system

For macOS users with Homebrew:
```bash
brew install llvm
brew install libomp
```

## Building

To compile the program, simply run:

```bash
make
```

This will create the executable `raytracer`.

## Usage

Run the program with:

```bash
./raytracer
```

The program will prompt you to enter:
1. Scene file name
2. Output image file name
3. Image width
4. Image height

Example input:
```
scene.txt output.ppm 800 600
```

## Scene File Format

The scene file should be structured as follows:

```
VP x y z           # Viewpoint coordinates
BG r g b           # Background color (RGB values 0-255)
OBJ_N n            # Number of objects (spheres)
S x y z r R G B    # Sphere definition (position, radius, color)
```

Example scene file:
```
VP 1.777 1 1
BG 255 255 255
OBJ_N 3
S 0.2 1 4 0.2 200 0 0
S 0 0 3 0.2 127 0 0
S 0 0 10 1 255 0 0
```

## Project Structure

- `main.c`: Main program logic and parallel rendering
- `scene.c/h`: Scene loading and ray-sphere intersection calculations
- `ppm.c/h`: PPM image handling with memory mapping

## Performance

The program uses OpenMP to parallelize the ray tracing computations across available CPU cores. Execution time is measured and reported in milliseconds.

## Memory Management

The program uses memory mapping for efficient image handling, which means:
- Large images can be handled without loading the entire file into memory
- Efficient writing of pixel data directly to disk
- Automatic memory management by the operating system

## Limitations

- Only supports sphere primitives
- Basic lighting model without shadows or reflections
- Fixed viewpoint position
- PPM format output only

## Cleanup

To clean the build files:

```bash
make clean
```
