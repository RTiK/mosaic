# Mosaic

Mosaic is an application that arranges icons on the home screen of your iOS device by color using evolutionary algorithms... Will be.

This is currently a work in progress and all it does right now is just arranging solid colors into home screen pages using a simple mutation.

## Installation

Everything is built with CMake. Before we begin installing the project we need to make sure the two dependencies OpenCV and GoogleTest are available on your computer.

### OpenCV

You can download the OpenCV source code and install it into the directory `3rdparty/opencv`. 

There are binaries available for Windows but make sure that they are made for your compiler. Otherwise you'll still have to compile them yourself. Also, for Windows don't forget to add OpenCV `bin/` directory (Debug and Release) to your `PATH` variable.
The [OpenCV installation guide](https://docs.opencv.org/master/d3/d52/tutorial_windows_install.htmlhttps://docs.opencv.org/master/d3/d52/tutorial_windows_install.html) is a good first choice. 

If you already have OpenCV installed somewhere on your computer the installation script will attempt to find a global installation as a fallback in case no installation could be found in the directory `3rdparty/opencv`. 

### GoogleTest

CMake will attempt to build the GTest library from source. This is considered best practice for using GTest. Please download the latest version of GTest into the directory `3rdparty/googletest`.

If no sources are available, the script will try to find the binaries in the `3rdparty/googletest` directory. As a final fallback the script will check for a system-wide installation.

### Mosaic

Many IDEs like CLion and Visual Studio will have a plugin for working with CMake projects.

If you wish to compile from command line you can issue the following commands from the project directory:

```bash
mkdir build && cd build
cmake ..
make
```

## Extracting

### Prerequisites

To get the icons that we want to arrange, we need to extract them from a screenshot of your home screen. But first, you will need to make some preparations.

#### Disable Parallax

Maybe you haven't noticed it, but the icons on your home screen move depending on the tilt of your device. Apple calls it the Parallax effect. The Extractor tool, however, needs to have the icons in their expected spot. Therefore, we need to disable this effect before we take the screenshot. To do that _enable_ the `Reduce Motion` option found under 

```
Settings > Accessibility > Motion > Reduce Motion
```

#### Clear all notifications bubbles

Another thing you need to make sure is that no icon has a red notification bubble because, of course, it will persist on the extracted icon.

#### Make the screenshot

When you have finished all steps above, you can take the screenshots of all your home screens and transfer them to your computer.

### Extracting icons

The `Extract` tool can be found in the directory `build/Extract/src/`. It takes a screenshot (PNG) file as input and extracts the icons from the home screen and the dock, and stores it in the destination folder.

```bash
# from the build directry
cd Extract/src

./Extract -n 16     # number of icons on the home screen page (default 24)
          -d 4      # number of icons in dock (default 4)
          -o page1/ # path to output folder (default current directory)
          path/to/screenshot.png
```

Expected icon locations are defined in `Extract/src/IconCenters.h`. Currently, only iPhones with screen size 1792x828 (e.g. Xr, 10, 11 etc.) are supported. 

## Analyzing

TODO

## Arranging

In the directory `examples` you will find the only example you can run right now

```bash
# from the build directory
cd Arrange/examples
./Example1
```

Setting `seed=0` will yield the following pages

|![example1 seed0](doc/page0.png) | ![example1 seed0](doc/page1.png) | ![example1 seed0](doc/page2.png)|
|---|---|---|