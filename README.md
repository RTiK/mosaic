# Mosaic

Mosaic is an application that arranges icons on the home screen of your iOS device by color using evolutionary algorithms.

> If you'd like a coding agent to help you, point it to AGENTS.md (if it fails to discover it on its own). The description in this file will help the agent build the project, run the example scripts and guide you through the process of extracting the icons from the screenshots and arranging them.

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

## How does it all work?

INTRO HERE

This is essentially a Traveling Salesman Problem disguised by a multi-objective scoring function:

- Each icon must fit well with its neighbors
- Each page must present a color theme

Optimizing for the first two objectives, the algorithm tends to group a small number of similar icons and distribute these groups over a large number of pages. To counter this, we need a third objective:

- Each page must be filled to a viable maximum

While there are many approaches to solve such problems, I have decided that it would be fun to do it with a genetic algorithm.

### Genetic algorithm

The initial input is a set of icons. The arrangement of these icons is a _Genome_ of an _Individual_.

In the beginning, a _Population_ of a fixed number of _Individuals_ is created. The _Individuals_ in the first generation all have a random arrangement of icons.

#### Strategy

With each generation increment a number of modifications is applied to the population. Some _Individuals_ die because they have reached the maximal age, some are passed into the new population without change, some are mutated and others die because they exhibit low overall fitness. Finally, the population is filled with new _Individuals_ with a randomized _Genome_.

#### Mutation

The only mutation implemented right now is swapping two icons in the _Genome_. This a simple operation that does not require to repair the _Genome_ afterward.

### Fitness function



We distinguish between two types of parameters: Fitness parameters and population parameters.