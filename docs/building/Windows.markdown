# Compiling for Windows

## Minimum requirements

The bare minimum for compiling mxitc is:

  * Qt 4.5
  * C++ compiler

However, we have provided build files for cmake only. If you wish to use another
build system (e.g. qmake), then feel free to make your own file.

If you aren't feeling so brave, below is a suggested build strategy.

## Suggested build strategy

Below is an example for building mxitc dynamically using only free software. If
you wish to use proprietry software such as Visual Studio, you may be able to
adopt this strategy with minimal modification.

We will use:
  * Qt 4.5 open-source edition
  * MingW32 (comes with Qt)
  * CMake to generate the MingW32 Makefile

### Requirements

  * Get a copy of the source code
  * Download and install Qt 4.5
  * Download and install CMake

### Compiling

  * Launch the CMake-gui
  * Select the location of the mxitc source
  * Select the location to build to (suggested: build/)
  * Click configure
  * Attend to any errors (you may need to locate qmake.exe)
  * Click configure again
  * Click generate
  * Select MingW32 files to generate
  * Run mingw32-make from the build directory (in command prompt or similar)

### Finally

Congratulations, you have built mxitc on Windows. You will need to ensure that
the Qt runtime DLL's are in your system PATH. Alternatively, ensure that all
runtime dependencies are in the same folder as mxitc.exe. These are:

  * mingwm10.dll
  * QtCore4.dll
  * QtGui4.dll
  * QtNetwork4.dll
  * QtWebKit4.dll

If you are missing any, mxitc.exe will report an error on startup.

To remove these runtime dependencies, please refer to the following URL:

  http://doc.trolltech.com/4.5/deployment-windows.html

We hope to post a walkthrough on how to do this in the future.
