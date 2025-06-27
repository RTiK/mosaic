# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Mosaic is a C++ application that uses evolutionary algorithms to arrange colored pieces (representing iOS app icons) on home screen pages. The goal is to optimize visual organization by minimizing color distances between adjacent pieces.

## Build System and Commands

### Building the Project
```bash
mkdir build && cd build
cmake ..
make
```

### Running Tests
```bash
# From build directory
./MosaicTests
```

### Running Examples
```bash
# From build directory
cd examples
./Example1
./Example2
```

### Dependencies
- **OpenCV**: Install in `3rdparty/opencv` or system-wide
- **GoogleTest**: Place sources in `3rdparty/googletest` (preferred) or install system-wide
- **CMake**: Minimum version 3.31
- **C++23** standard required

## Architecture

### Core Design Pattern
The project implements a **hierarchical evolutionary algorithm** with three main levels:

1. **Individual** - Complete solution containing genome and fitness
2. **Page** - iOS-style 4x6 grid (24 pieces max) with eager fitness evaluation
3. **Piece** - Abstract base class for colored elements (Strategy pattern)

### Key Classes

- **Individual**: Represents complete solution with genome (vector of Pieces) and derived pages
- **Page**: Manages up to 24 pieces in 4x6 grid, tracks fitness metrics (distances, variance, color means)
- **Piece**: Abstract interface for all piece types (pure virtual distance, image generation, color extraction)
- **ColorPiece**: Concrete piece implementation using RGB color values
- **PageEvaluation**: Namespace with fitness functions considering neighbor relationships
- **PopulationUtil**: Evolutionary algorithm operations (selection, mutation, population management)

### Important Design Decisions

- **Eager Evaluation**: Fitness computed immediately after genome changes to ensure const-correctness
- **Neighbor Weighting**: Diagonal neighbors weighted at 0.70711 (√2/2) for spatial accuracy
- **iOS Layout**: 4x6 grid directly models iOS home screen pages
- **OpenCV Integration**: Used for color processing and image generation

### File Structure

- `include/Mosaic/` - Public headers
- `src/` - Implementation files
- `tests/` - Unit tests using GoogleTest
- `examples/` - Usage examples
- `3rdparty/` - External dependencies (OpenCV, GoogleTest)

## Development Notes

- The project uses C++23 features
- All fitness evaluation is performed eagerly to maintain const-correctness
- Color distances use Euclidean distance in RGB space
- Page layout constants are defined in PageEvaluation namespace
- Individual comparison operators enable sorting in std::set containers