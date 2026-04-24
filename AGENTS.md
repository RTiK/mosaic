# AGENTS.md — Mosaic

This file is a guide for coding agents (Claude Code, OpenAI Codex, Gemini CLI, Cursor, etc.) and developers working with Mosaic. It covers architecture, build, configuration, running, and visualization.

## What Mosaic Does

Mosaic is a C++ evolutionary algorithm that takes a collection of iOS app icons and arranges them across home screen pages so that visually similar icons end up on the same page. It minimizes color distances between neighboring icons using perceptual (CIE Lab) color space.

---

## Architecture

### Core Design Pattern

The project implements a **hierarchical evolutionary algorithm** with three main levels:

1. **Individual** - Complete solution containing genome and fitness
2. **Page** - iOS-style 4x6 grid (24 pieces max) with eager fitness evaluation
3. **Piece** - Abstract base class for colored elements (Strategy pattern)

### Key Classes

- **Individual**: Represents complete solution with genome (vector of Pieces), derived pages, fitness, and birth_generation
- **Page**: Manages up to 24 pieces in 4x6 grid, tracks fitness metrics (distances, variance, color distribution, icons_missing)
- **Piece**: Abstract interface for all piece types (pure virtual distance, image generation, color extraction)
  - **ColorPiece**: Simple BGR color implementation
  - **LabPiece**: Extends ColorPiece with LAB color space support
  - **IconPiece**: Base class for icon-based pieces (BGRA image loading)
  - **LabIconPiece**: Real iOS icons with k-means clustering in LAB color space for perceptual color distance
- **IndividualGeneration**: Namespace with factory functions (`GenerateIndividualLabRandom`, `GenerateIndividualGrayRandom`, `ReadRgbIcons`)
- **PageEvaluation**: Namespace with fitness functions considering neighbor relationships
- **PopulationUtil**: Evolutionary algorithm operations (selection, mutation, `FilterByAge`)
- **HallOfFame**: Maintains top N individuals across all generations with callback hooks
- **JsonExport**: Exports individuals to NDJSON format; uses `PieceType` enum (`COLOR_PIECE`, `LAB_PIECE`, `LAB_ICON_PIECE`)

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
  - `piece/` - Piece implementations (ColorPiece, LabPiece, IconPiece, LabIconPiece)
- `src/` - Implementations
- `examples/RealIconExample.cpp` - Main entry point for real-icon runs — the file to edit
- `tests/` - GoogleTest unit tests
- `visualization/` - Jupyter notebook for result analysis
- `doc/JSON_EXPORT.md` - NDJSON schema and analysis guide
- `3rdparty/` - External dependencies (OpenCV, GoogleTest)

Key namespaces: `individual_generation`, `population_util`, `page_evaluation`, `json_export`
Key classes: `Individual`, `Page`, `HallOfFame`, `LabIconPiece`

---

## Build System and Commands

### Dependencies
- **OpenCV**: Install in `3rdparty/opencv` or system-wide
- **GoogleTest**: Auto-fetched via CMake FetchContent (no manual install needed)
- **nlohmann/json**: Auto-fetched via CMake FetchContent (v3.11.3)
- **CMake**: Minimum version 3.31
- **C++23** standard required
- **Python 3** (optional): For visualization tools in `visualization/`

### Building the Project
```bash
mkdir build && cd build
cmake ..
make -j8
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
./GrayscaleIconsExample   # Basic usage (grayscale ColorPieces)
./SolidColorIconsExample  # Lab color arrangement with JSON export
./RealIconExample         # Real icon arrangement from filesystem
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

---

## End-to-End User Workflow

### Step 1 — Extract Icons from Home Screen Screenshots

Use the tool **iOS Icon Extractor**: https://github.com/RTiK/iOS-icon-extractor.

Clone and set up that repository first, then follow its `AGENTS.md` for the full extraction process.

Before extracting, choose an output directory (e.g. `icons/`). If that directory already exists and contains files, ask the user whether to delete the existing files or use a different directory — leftover files from a previous run will be picked up by Mosaic alongside the new ones.

### Step 2 — Review and Curate Icons

Before arranging, give the user a chance to review what was extracted and remove any icons they don't want included.

**Dock icons**: The dock is not part of the arrangement — icons the user keeps in the dock should be removed from the icons directory so Mosaic doesn't try to place them on a page. Ask the user which icons they want in their dock and delete those files.

**Unwanted icons**: The user may also want to remove any other icons they don't want arranged (e.g. apps they plan to delete).

Once the user is happy with the contents of the icons directory, proceed to the next step.

### Step 3 — Build Mosaic

```bash
git clone <this-repo>
cd mosaic
mkdir build && cd build
cmake ..
make -j8
```

Dependencies are handled automatically by CMake (GoogleTest, nlohmann/json). Only **OpenCV** must be installed separately (system-wide or placed in `3rdparty/opencv`).

### Step 4 — Configure the Run

The main entry point for real icons is `examples/RealIconExample.cpp`. Edit the constants at the top of that file before running:

```cpp
const int kPopulation = 200;     // population size
const int kGenerations = 2000;   // how many evolution cycles to run
const int kPageBreaks = 3;       // max extra page-break genes in the genome
const int kMaxAge = 50;          // how long an individual survives before dying

const FitnessWeights kFitnessWeights = {
  .variance_weight = 1.5,        // penalty for color spread within a page
  .missing_icons_weight = 0.5    // penalty for underfilled pages
};

const std::string kIconDirPath = "../../icons";  // path to extracted icons
```

Point `kIconDirPath` at the directory produced by iOS Icon Extractor (absolute or relative to `build/examples/`).

### Step 5 — Run the Example

```bash
cd build/examples
./RealIconExample
```

The algorithm prints fitness scores each generation. Progress is also saved to `hall_of_fame_progress.ndjson` in the working directory. Use Ctrl+C to stop early — the file is crash-safe and already contains the best individuals found so far.

### Step 6 — Visualize Results (Optional)

```bash
cd visualization
python3 -m venv venv
source venv/bin/activate
pip install -r requirements.txt
jupyter notebook mosaic_analysis.ipynb
```

Point the notebook at the `.ndjson` file to see the arranged pages as images.

---

## Parameter Tuning Guide

### `kPageBreaks` — Maximum Page Breaks

A page break is a special gene in the genome that signals "start a new page here." The genome is a flat list of icons and page breaks; fitness evaluation splits it into pages at each page break.

- More page breaks → more (potentially smaller) pages, more flexibility in grouping
- Too many page breaks → many underfilled pages (penalized by `missing_icons_weight`)
- Page breaks can mutate to the start/end of the genome, effectively reducing the actual page count. `kPageBreaks` is therefore a **maximum** — the algorithm may use fewer.
- **Rule of thumb**: start with `kPageBreaks = number_of_icons / 24` (one per full page) and adjust based on results.

### `kGenerations` — Number of Generations

How many evolution cycles run before the algorithm stops. The population converges over time.

- Too few → suboptimal groupings, population hasn't had time to explore
- Too many → wasted compute once converged (watch the fitness curve in the notebook)
- **Rule of thumb**: start with 1000–2000. Watch whether the best fitness is still improving in the final 20% of generations; if not, it has converged and you can stop earlier.

### `kMaxAge` — Maximum Individual Age

How many generations an individual can survive before being removed, even if it is elite.

- This prevents the population from being dominated by a few old individuals that block exploration of new solutions.
- **Rule of thumb**: `kMaxAge = kGenerations / 40` is a reasonable starting point.

### `kPopulation` — Population Size

Total number of individuals kept each generation. Each generation:
- Top 10% pass through unchanged (elites)
- Next 60% are mutated (two random genes swap)
- Remaining ~30% are randomly generated fresh individuals

Larger populations explore more of the search space but run slower. For a typical iOS device with ~100–200 icons, 200 individuals works well.

### `FitnessWeights` — Balancing the Fitness Function

These two weights are the core tuning knobs:

**`variance_weight`** — penalizes color spread within a page. Higher values push the algorithm toward pages where all icons have similar colors. This is the primary objective.

**`missing_icons_weight`** — penalizes underfilled pages. Without this, the algorithm would put 2–3 very similar icons on a page, achieve near-zero variance, and ignore the rest. This weight forces it to fill pages.

The two weights push against each other. If your results show:
- Pages are too sparse (many half-empty pages) → increase `missing_icons_weight`
- Icons on a page still look visually mixed → increase `variance_weight`
- The algorithm converges to trivial solutions (1–2 icons per page) → `missing_icons_weight` is too low relative to `variance_weight`

**Default starting values**: `variance_weight = 1.5`, `missing_icons_weight = 0.5`

---

## Export and Analysis Workflow

### Exporting Individuals

Use the `JsonExport` module to export individuals during or after evolution:

```cpp
#include <Mosaic/JsonExport.hpp>

// Export an individual to NDJSON file
json_export::ExportIndividualToNDJSON(
    individual,
    "results.ndjson",
    json_export::PieceType::LAB_ICON_PIECE
);
```

### Hall of Fame Integration

Export best individuals automatically using callbacks:

```cpp
HallOfFame hof(10);  // Track top 10
hof.SetOnInsertCallback([](const Individual& ind, size_t rank) {
    // rank is 1-based
    json_export::ExportIndividualToNDJSON(ind, "best-individuals.ndjson", json_export::PieceType::LAB_ICON_PIECE);
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

---

## Development Notes

- The project uses C++23 features
- All fitness evaluation is performed eagerly to maintain const-correctness
- Color distances use Euclidean distance in RGB/BGR or LAB color space (depending on Piece type)
- LAB color space provides perceptually uniform distances compared to RGB
- Page layout constants are defined in PageEvaluation namespace
- Individual comparison operators enable sorting in std::set containers
- Pieces can return weighted color distributions via k-means clustering for richer color representation
