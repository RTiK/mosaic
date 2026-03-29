# AGENTS.md — Mosaic

This file is a guide for coding agents (Claude Code, OpenAI Codex, Gemini CLI, Cursor, etc.) helping a user run Mosaic end-to-end.

## What Mosaic Does

Mosaic is a C++ evolutionary algorithm that takes a collection of iOS app icons and arranges them across home screen pages so that visually similar icons end up on the same page. It minimizes color distances between neighboring icons using perceptual (CIE Lab) color space.

---

## End-to-End User Workflow

### Step 1 — Extract Icons from Home Screen Screenshots

Use the tool **iOS Icon Extractor**: https://github.com/RTiK/iOS-icon-extractor.

Take screenshots of all your home screen pages on your iPhone and transfer them to your computer first. The output is a flat directory of `.png` files — that directory path is what Mosaic needs.

### Step 2 — Build Mosaic

```bash
git clone <this-repo>
cd mosaic
mkdir build && cd build
cmake ..
make -j8
```

Dependencies are handled automatically by CMake (GoogleTest, nlohmann/json). Only **OpenCV** must be installed separately (system-wide or placed in `3rdparty/opencv`).

### Step 3 — Configure the Run

The main entry point for real icons is `examples/RealIconExample.cpp`. Edit the constants at the top of that file before running:

```cpp
const int kPopulation = 200;     // population size
const int kGenerations = 2000;   // how many evolution cycles to run
const int kPageBreaks = 3;       // max extra page-break genes in the genome
const int kMaxAge = 50;          // how long an individual survives before dying

const FitnessWeights kFitnessWeights = {
  .variance_weight = 1.9,        // penalty for color spread within a page
  .missing_icons_weight = 0.4    // penalty for underfilled pages
};

const std::string kIconDirPath = "../../icons";  // path to extracted icons
```

Point `kIconDirPath` at the directory produced by iOS Icon Extractor (absolute or relative to `build/examples/`).

### Step 4 — Run the Example

```bash
cd build/examples
./Example3
```

The algorithm prints fitness scores each generation. Progress is also saved to `hall_of_fame_progress.ndjson` in the working directory. Use Ctrl+C to stop early — the file is crash-safe and already contains the best individuals found so far.

### Step 5 — Visualize Results (Optional)

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

**Default starting values**: `variance_weight = 1.9`, `missing_icons_weight = 0.4`

---

## Output Format

Mosaic exports results to NDJSON (newline-delimited JSON). Each line is one individual snapshot. The file is written incrementally whenever the Hall of Fame improves, so it is safe to interrupt the run.

See `doc/JSON_EXPORT.md` for the full schema and `jq` query examples.

---

## Project Layout (for Agents)

```
include/Mosaic/         Public headers — start here to understand the API
src/                    Implementations
examples/Example3.cpp   Main entry point for real-icon runs — the file to edit
tests/                  GoogleTest unit tests
visualization/          Jupyter notebook for result analysis
doc/JSON_EXPORT.md      NDJSON schema and analysis guide
```

Key namespaces: `individual_generation`, `population_util`, `page_evaluation`, `json_export`
Key classes: `Individual`, `Page`, `HallOfFame`, `LabIconPiece`
