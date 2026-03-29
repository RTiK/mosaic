# Mosaic

Mosaic is an application that arranges icons on the home screen of your iOS device by color using evolutionary algorithms.

> If you'd like a coding agent to help you, point it to AGENTS.md (if it fails to discover it on its own). The description in this file will help the agent build the project, run the example scripts and even take the icons from your screenshots and arrange them.

## Constrains

This library is only made for arranging icons on the classic screen layouts. The screens are filled with icons from left to right and from top to bottom. Just as Steve Jobs intended. There is currently no support for widgets or arbitrary icon placement that was introduced in iOS 18.

## Installation

Everything is built with CMake. Image processing is done by OpenCV, and for testing we are using Google Test. Many IDEs like CLion and Visual Studio will have a plugin for working with CMake projects.

If you wish to compile from the command line, you can issue the following commands from the project directory:

```bash
mkdir build && cd build
cmake ..
make
```

## Examples

In the directory `examples` you will find three scripts.

- `GrayscaleIconsExample` creates icons with random grayscale colors and arranges them.
- `ColorIconsExample` creates icons with random colors and arranges them.
- `RealIconExample` reads images of real icons from a directory and arranges them.

Setting `seed=0` in `RealIconExample` will yield the following pages

![icons](doc/icons.png)

## 

## Parameters

So how does it work exactly? Well, let's assume you've got 32 icons on your home screen (excluding the dock) and you want to arrange them by color. Trying out every permutation would yield 32! possible options. That's 2.6*10^35. Good luck brute forcing that one. 