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
./Example1              # Basic usage
./Example2              # Intermediate example
./Example3              # Advanced evolution example
./Example3_JsonExport   # Evolution with JSON export
./TestJsonExport        # JSON export demonstration
```

### Visualization and Analysis
```bash
# Set up Python environment for visualization
cd visualization
python3 -m venv venv
source venv/bin/activate
pip install -r requirements.txt

# Run Jupyter notebook for analyzing exported individuals
jupyter notebook mosaic_analysis.ipynb
```

### Dependencies
- **OpenCV**: Install in `3rdparty/opencv` or system-wide
- **GoogleTest**: Place sources in `3rdparty/googletest` (preferred) or install system-wide
- **CMake**: Minimum version 3.31
- **C++23** standard required
- **Python 3** (optional): For visualization tools in `visualization/`

## Architecture

### Core Design Pattern
The project implements a **hierarchical evolutionary algorithm** with three main levels:

1. **Individual** - Complete solution containing genome and fitness
2. **Page** - iOS-style 4x6 grid (24 pieces max) with eager fitness evaluation
3. **Piece** - Abstract base class for colored elements (Strategy pattern)

### Key Classes

- **Individual**: Represents complete solution with genome (vector of Pieces) and derived pages
- **Page**: Manages up to 24 pieces in 4x6 grid, tracks fitness metrics (distances, variance, color means, color distribution)
- **Piece**: Abstract interface for all piece types (pure virtual distance, image generation, color extraction)
  - **ColorPiece**: Simple RGB color implementation
  - **BgrIconPiece**: Real iOS icons using BGR color space
  - **LabIconPiece**: Icons using LAB color space for perceptual color distance
  - **LabIconClusteringPiece**: LAB-based with k-means color clustering
  - **IconPiece**: Base class for icon-based pieces
- **PageEvaluation**: Namespace with fitness functions considering neighbor relationships
- **PopulationUtil**: Evolutionary algorithm operations (selection, mutation, population management)
- **HallOfFame**: Maintains top N individuals across all generations with callback hooks
- **JsonExport**: Exports individuals to NDJSON format for analysis and visualization

### Important Design Decisions

- **Eager Evaluation**: Fitness computed immediately after genome changes to ensure const-correctness
- **Neighbor Weighting**: Diagonal neighbors weighted at 0.70711 (√2/2) for spatial accuracy
- **iOS Layout**: 4x6 grid directly models iOS home screen pages
- **OpenCV Integration**: Used for color processing and image generation
- **Weighted Color Distribution**: Pieces return quantified color distributions (k-means clusters with weights) rather than single colors
- **Color Space Options**: Support for both RGB/BGR (device) and LAB (perceptual) color spaces
- **Hall of Fame Callbacks**: Hooks allow automatic export/logging when best individuals are discovered
- **NDJSON Export Format**: Crash-safe, appendable format for tracking evolution progress

### File Structure

- `include/Mosaic/` - Public headers
  - `piece/` - Piece implementations (ColorPiece, BgrIconPiece, LabIconPiece, etc.)
- `src/` - Implementation files
- `tests/` - Unit tests using GoogleTest
- `examples/` - Usage examples and demonstrations
- `visualization/` - Python/Jupyter tools for analyzing exported data
- `doc/` - Documentation (JSON export schema, analysis guides)
- `3rdparty/` - External dependencies (OpenCV, GoogleTest)

## Export and Analysis Workflow

### Exporting Individuals

Use the `JsonExport` module to export individuals during or after evolution:

```cpp
#include <Mosaic/JsonExport.hpp>

// Configure export parameters
json_export::ExportConfig config;
config.diagonal_weight = 0.70711;
config.page_width = 4;
config.page_height = 6;

// Export an individual to NDJSON file
json_export::ExportIndividualToNDJSON(
    individual,
    "results.ndjson",
    config,
    "BgrIconPiece"
);
```

### Hall of Fame Integration

Export best individuals automatically using callbacks:

```cpp
HallOfFame hof(10);  // Track top 10
hof.SetOnInsertCallback([&config](const Individual& ind, size_t rank) {
    json_export::ExportIndividualToNDJSON(ind, "best-individuals.ndjson", config, "BgrIconPiece");
});

// Updates automatically trigger exports
hof.Update(population);
```

### Analyzing Results

See `doc/JSON_EXPORT.md` for complete documentation on:
- Using `jq` for command-line analysis
- Python scripts for visualization
- Understanding the JSON schema
- Debugging problematic placements

## Development Notes

- The project uses C++23 features
- All fitness evaluation is performed eagerly to maintain const-correctness
- Color distances use Euclidean distance in RGB/BGR or LAB color space (depending on Piece type)
- LAB color space provides perceptually uniform distances compared to RGB
- Page layout constants are defined in PageEvaluation namespace
- Individual comparison operators enable sorting in std::set containers
- Pieces can return weighted color distributions via k-means clustering for richer color representation