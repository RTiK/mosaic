# JSON Export Documentation

This document describes the JSON export functionality for the Mosaic evolutionary algorithm.

## Overview

The Mosaic project includes a JSON export system that serializes complete individuals (solutions) to NDJSON format. This allows you to:

1. **Debug specific placement decisions** - See exactly why pieces are placed where they are
2. **Build visualization tools** - Create custom viewers for analyzing results
3. **Track evolution progress** - Export individuals periodically during algorithm runs
4. **Verify fitness calculations** - All raw data and calculated metrics are included

## File Format: NDJSON

Exported data uses **NDJSON (Newline Delimited JSON)** format:
- Each line is a complete, valid JSON object representing one individual
- Lines can be appended during evolution without rewriting the file
- Files are crash-safe (incomplete last line can be discarded)
- Easy to process with standard tools (`jq`, Python, etc.)

Example file with 3 individuals:
```
{"fitness": 123.45, "generation": 10, ...}
{"fitness": 98.76, "generation": 15, ...}
{"fitness": 145.23, "generation": 20, ...}
```

## Usage

### C++ Export

```cpp
#include <Mosaic/JsonExport.hpp>

// Configure export parameters
json_export::ExportConfig config;
config.diagonal_weight = 0.70711;  // Weight for diagonal neighbors
config.page_width = 4;              // iOS page width
config.page_height = 6;             // iOS page height

// Export an individual
json_export::ExportIndividualToNDJSON(
    individual,           // The Individual to export
    "results.ndjson",    // Output file (will be created or appended)
    config,              // Configuration
    "LabIconPiece"       // Piece type identifier
);
```

### Analyzing Exported Data

#### Using `jq` (command-line)

```bash
# Pretty-print first individual
cat results.ndjson | head -1 | jq '.'

# View summary of all individuals
cat results.ndjson | jq '{fitness, generation, num_pages: (.pages | length)}'

# Examine a specific piece
cat results.ndjson | head -1 | jq '.pages[0].pieces[0]'

# Find pieces with high neighbor distances (potential problem areas)
cat results.ndjson | head -1 | jq '
  .pages[].pieces[] |
  select(.avg_neighbor_distance > 0.8) |
  {genome_index, page_position, avg_neighbor_distance, icon_path}
'

# Compare page fitness metrics
cat results.ndjson | head -1 | jq '
  .pages[] |
  {page_index, total_distances, variance, icons_missing}
'
```

#### Using Python

```python
import json

# Read all individuals
individuals = []
with open('results.ndjson', 'r') as f:
    for line in f:
        individuals.append(json.loads(line))

# Get best individual
best = min(individuals, key=lambda ind: ind['fitness'])

# Analyze a specific piece's neighbors
page = best['pages'][0]
piece = page['pieces'][5]  # 6th piece on first page

print(f"Piece at genome index {piece['genome_index']}")
print(f"Main color: {piece['main_color']}")
print(f"Avg distance to neighbors: {piece['avg_neighbor_distance']:.4f}")
print(f"\nNeighbors:")
for neighbor in piece['neighbors']:
    print(f"  {neighbor['direction']:3s}: distance={neighbor['distance']:.4f}, "
          f"weighted={neighbor['weighted_distance']:.4f}")
```

## JSON Schema

The complete JSON schema is available in [`individual_export_schema.json`](individual_export_schema.json). This can be used for:

- **Validation**: Use JSON schema validators to verify exported data
- **Documentation**: Reference for building tools that read the exports
- **IDE Support**: Many IDEs provide autocomplete when a schema is available

### Key Structure

```
Individual
├── fitness (number)              - Total fitness score
├── generation (integer)          - Birth generation
├── piece_type (string)           - Type of all pieces
├── config (object)               - Algorithm parameters
├── genome_order (array)          - Linear genome structure
└── pages (array)
    └── Page
        ├── page_index (integer)
        ├── total_distances (number)   - Sum of neighbor distances
        ├── variance (number)           - Color variance from mean
        ├── icons_missing (integer)
        ├── mean_color (array)
        ├── color_distribution (array)
        └── pieces (array)
            └── Piece
                ├── genome_index (integer)
                ├── page_position (integer)
                ├── row, column (integer)
                ├── icon_path (string, optional)
                ├── main_color (array)
                ├── quantified_colors (array)
                ├── neighbors (array)
                │   └── Neighbor
                │       ├── direction (string)
                │       ├── neighbor_genome_index
                │       ├── neighbor_page_position
                │       ├── distance (number)
                │       ├── weight (number)
                │       └── weighted_distance (number)
                └── avg_neighbor_distance (number)
```

## Debugging Workflow

### Finding Problematic Placements

1. **Export individuals during evolution**
   ```cpp
   if ((generation + 1) % 10 == 0) {
       auto best = *population.begin();
       json_export::ExportIndividualToNDJSON(best, "evolution.ndjson", config, "LabIconPiece");
   }
   ```

2. **Identify pieces with high neighbor distances**
   ```bash
   cat evolution.ndjson | tail -1 | jq '
     [.pages[].pieces[] | {genome_index, avg_neighbor_distance, icon_path}] |
     sort_by(.avg_neighbor_distance) |
     reverse |
     .[0:5]
   '
   ```

3. **Examine specific neighbor relationships**
   - Look at the `neighbors` array for a problematic piece
   - Check `distance` and `weighted_distance` values
   - Compare with what you'd expect visually

4. **Check page-level metrics**
   - High `variance` = colors are spread out
   - High `total_distances` = pieces don't match their neighbors well
   - Compare `color_distribution` across pages

### Building a Visualizer

You can build a Python/JavaScript tool that:

1. Loads the NDJSON file
2. Displays pages as grids with actual colors
3. Highlights pieces when clicked, showing:
   - All neighbors with their distances
   - How fitness would change if moved to different pages
   - Color clustering in RGB/LAB space

The schema provides all necessary data - no need to recompute anything.

## Examples

See the `examples/` directory:
- **TestJsonExport.cpp** - Simple export example
- **Example3_JsonExport.cpp** - Export during evolution

## Technical Notes

### Color Representation

Colors are exported as arrays `[c1, c2, c3]` where the meaning depends on `piece_type`:
- `LabIconPiece`: LAB color space, L in [0, 100], a/b typically [-128, 127]
- Other piece types: See piece implementation

### Neighbor Weighting

- Cardinal neighbors (N, S, E, W): weight = 1.0
- Diagonal neighbors (NE, SE, SW, NW): weight = 0.70711 (√2/2)
- Diagonal weight accounts for greater spatial distance

### Fitness Calculation

Individual fitness combines:
- Page distances (sum of all `total_distances`)
- Variance (sum of all `variance` × 0.4)
- Missing icons penalty (based on `icons_missing`)

The exact formula is in `Individual::Evaluate()`.

## Future Enhancements

Possible additions to the export format:
- Alternative placement suggestions for problematic pieces
- Historical lineage (parent IDs)
- Mutation history
- Per-generation statistics
