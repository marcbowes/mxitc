# Compiling for Windows

## Minimum requirements

The bare minimum for compiling mxitc is:

  * Qt 4.5
  * C++ compiler

However, we have provided build files for cmake only. If you wish to use another
build system (e.g. qmake), then feel free to make your own file.

If you aren't feeling so brave, below is a suggested build strategy.

## Suggested build strategy

Below is an example for building mxitc dynamically using:

  * Qt 4.5
  * The GNU C++ compiler
  * CMake

### Requirements

  * Get a copy of the source code
  * Download and install G++
  * Download and install Qt 4.5
  * Download and install CMake

### Compiling

  * `cd /path/to/source/build`
  * `cmake ..`
  * You may configure cmake with `ccmake ..` (e.g. change build type)
  * `make`

### Finally

The compiled binary will be in build/src/ under the name mxitc.
