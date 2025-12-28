#include "Mosaic/PageEvaluation.hpp"
#include "Mosaic/piece/ColorPiece.hpp"
#include <map>
#include <tuple>
#include <algorithm>
#include <cmath>

float page_evaluation::CalculatePageDistances(const Page &page) {
  unsigned int total_pieces = page.Size();

  double total_distance = 0.0;

  for (std::shared_ptr<Piece> *current = page.GetFirstPiece(); current <= page.GetLastPiece(); current++) {
    unsigned int piece_index = current - page.GetFirstPiece();
    unsigned char neighbours = page_evaluation::CalculateNeighbors(piece_index, total_pieces);

    Piece *current_piece = current->get();

    double piece_distance = 0.0;

    if (neighbours & Neighbours::N) {
      piece_distance += current_piece->Distance(**(current - kWidth));
    }
    if (neighbours & Neighbours::NE) {
      piece_distance += current_piece->Distance(**(current - (kWidth - 1))) * kDiagonalWeight;
    }
    if (neighbours & Neighbours::E) {
      piece_distance += current_piece->Distance(**(current + 1));
    }
    if (neighbours & Neighbours::SE) {
      piece_distance += current_piece->Distance(**(current + (kWidth + 1))) * kDiagonalWeight;
    }
    if (neighbours & Neighbours::S) {
      piece_distance += current_piece->Distance(**(current + kWidth));
    }
    if (neighbours & Neighbours::SW) {
      piece_distance += current_piece->Distance(**(current + (kWidth - 1))) * kDiagonalWeight;
    }
    if (neighbours & Neighbours::W) {
      piece_distance += current_piece->Distance(**(current - 1));
    }
    if (neighbours & Neighbours::NW) {
      piece_distance += current_piece->Distance(**(current - (kWidth + 1))) * kDiagonalWeight;
    }

    total_distance += neighbours > 0
        ? piece_distance / SumUpNeighbours(neighbours)
        : FLT_MAX;  // having no neighbours means that we have a single piece on page and we want to avoid that
  }
  
  return total_distance;
}

float page_evaluation::SumUpNeighbours(unsigned char neighbours) {
  unsigned char direct_neighbors = neighbours & (Neighbours::N | Neighbours::E | Neighbours::S | Neighbours::W);
  unsigned char diagonal_neighbors = neighbours & (Neighbours::NE | Neighbours::SE | Neighbours::SW | Neighbours::NW);
  float direct_neighbors_count = static_cast<float>(std::bitset<8>(direct_neighbors).count());
  float diagonal_neighbors_count = static_cast<float>(std::bitset<8>(diagonal_neighbors).count());
  return direct_neighbors_count + diagonal_neighbors_count * kDiagonalWeight;
}

std::vector<WeightedColor> page_evaluation::CalculateColorDistribution(const Page &page) {
  // Collect all dominant colors from all pieces with their weights
  std::map<std::tuple<float, float, float>, float> color_weight_map;

  for (std::shared_ptr<Piece> *current = page.GetFirstPiece(); current <= page.GetLastPiece(); current++) {
    std::vector<WeightedColor> piece_colors = (**current).GetQuantifiedColors();
    for (const auto& dc : piece_colors) {
      // Use color as key - round to 4 decimal places to avoid floating point issues
      float r0 = std::round(dc.color[0] * 10000.0f) / 10000.0f;
      float r1 = std::round(dc.color[1] * 10000.0f) / 10000.0f;
      float r2 = std::round(dc.color[2] * 10000.0f) / 10000.0f;
      auto key = std::make_tuple(r0, r1, r2);
      color_weight_map[key] += dc.weight;
    }
  }

  // Convert map to vector and normalize weights
  std::vector<WeightedColor> result;
  float total_weight = 0.0f;
  for (const auto& [color_tuple, weight] : color_weight_map) {
    total_weight += weight;
  }

  // Avoid division by zero
  if (total_weight == 0.0f) {
    return result;  // Return empty vector
  }

  for (const auto& [color_tuple, weight] : color_weight_map) {
    cv::Vec3f color(std::get<0>(color_tuple), std::get<1>(color_tuple), std::get<2>(color_tuple));
    result.push_back({color, weight / total_weight});
  }

  // Sort by weight (most dominant first)
  std::sort(result.begin(), result.end(), [](const WeightedColor& a, const WeightedColor& b) {
    return a.weight > b.weight;
  });

  return result;
}

float page_evaluation::CalculateVariance(const Page &page) {
  // Calculate simple mean of dominating colors (for backward compatibility)
  cv::Vec3f mean_color(0.0f, 0.0f, 0.0f);
  int num_pieces = 0;
  for (std::shared_ptr<Piece> *current = page.GetFirstPiece(); current <= page.GetLastPiece(); current++) {
    mean_color += (**current).GetMainColor();
    num_pieces++;
  }
  mean_color /= static_cast<float>(num_pieces);

  // Calculate variance as distance from mean
  double total_distance = 0.0;
  for (std::shared_ptr<Piece> *current = page.GetFirstPiece(); current <= page.GetLastPiece(); current++) {
    total_distance += ColorPiece::EuclideanDistance((**current).GetMainColor(), mean_color);
  }
  return total_distance;
}

unsigned char page_evaluation::CalculateNeighbors(unsigned int piece_index, unsigned int total_pieces) {
  unsigned int piece_index_end = total_pieces - 1 - piece_index;
  unsigned int neighbours = Neighbours::ALL;

  if (piece_index % kWidth == 0) {  // position on far left -> can't go west
    neighbours &= ~(Neighbours::NW | Neighbours::W | Neighbours::SW);
  } else if (piece_index % kWidth == kWidth - 1) {  // position on far right -> can't go east
    neighbours &= ~(Neighbours::SE | Neighbours::E | Neighbours::NE);
  }

  if (piece_index < kWidth) {  // can't go north
    neighbours &= ~(Neighbours::NW | Neighbours::N | Neighbours::NE);
  }

  if (piece_index_end < kWidth - 1) {  // can't go south
    neighbours &= ~(Neighbours::SW | Neighbours::S | Neighbours::SE);
  }

  if (piece_index_end == kWidth - 1) {
    neighbours &= ~(Neighbours::S | Neighbours::SE);
  }

  if (piece_index_end == kWidth) {
    neighbours &= ~Neighbours::SE;
  }

  if (piece_index_end == 0) {
    neighbours &= ~(Neighbours::E | Neighbours::SE | Neighbours::S | Neighbours::SW);
  }

  return neighbours;
}

int page_evaluation::CalculateIconsMissing(const Page &page) {
  return kHeight * kWidth - page.Size();
}
