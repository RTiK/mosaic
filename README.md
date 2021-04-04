# Mosaic

Mosaic is an application that arranges icons on the home screen of your iOS device by color using evolutionary algorithms... Will be.

This is currently a work in progress and all it does right now is just arranging solid colors into home screen pages using a simple mutation.

## Installation

Everything is compiled with CMake. Before we begin installing the project we need to make sure we have the two dependencies: OpenCV and GoogleTest.

### OpenCV

You can download the OpenCV source code, copy it into the directory `3rdparty/opencv` and install OpenCV there. There are binaries available for Windows but make sure that they are made for your compiler. Otherwise you'll still have to compile them with yourself. Finally, make sure to include the OpenCV ´bin/´ directory (Debug and Release) in your ´PATH´ variable.
The [OpenCV installation guide](https://docs.opencv.org/master/d3/d52/tutorial_windows_install.htmlhttps://docs.opencv.org/master/d3/d52/tutorial_windows_install.html) is a good first choice. 

If you already have OpenCV installed somewhere on your computer the installation script will attempt to find it as a fallback in case no installation could be found in the directory `3rdparty/opencv`. 

### GoogleTest

The installation script will attempt to compile the GTest library from sources in the directory `3rdparty/googletest`. This is considered best practice using GTest.

If no sources are available, the script will try to find the binaries in the `3rdparty/googletest` directory. As a final fallback the script will check for a system-wide installation.

### Mosaic

Many IDEs like CLion and Visual Studio will have a plugin for working with CMake projects.

If you wish to compile from command line you can issue the following commands from the project directory:

```bash
mkdir build && cd build
cmake ..
make
```

## Running

In the directory `examples` you will find the only example you can run right now

```bash
# from the build directory
cd examples
./Example1
```