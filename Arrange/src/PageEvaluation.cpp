//
// Created by Artem Khatchatourov on 22.04.21.
//

#include "PageEvaluation.h"

float page_evaluation::CalculatePageDistances(const Page &page) {
  unsigned int total_pieces = page.Size();

  double total_fitness = 0.0;

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

    total_fitness += neighbours > 0
        ? piece_distance / SumUpNeighbours(neighbours)
        : FLT_MAX;  // having no neighbours means that we have a single piece on page and we want to avoid that
  }
  return total_fitness;
}

float page_evaluation::SumUpNeighbours(unsigned char neighbours) {
  unsigned char normal_neighbors = neighbours & (Neighbours::N | Neighbours::E | Neighbours::S | Neighbours::W);
  unsigned char diagonal_neighbors = neighbours & (Neighbours::NE | Neighbours::SE | Neighbours::SW | Neighbours::NW);
  float normal_neighbors_count = (float) std::bitset<8>(normal_neighbors).count();
  float diagonal_neighbors_count = (float) std::bitset<8>(diagonal_neighbors).count();
  return normal_neighbors_count + diagonal_neighbors_count * kDiagonalWeight;
}

cv::Vec3f page_evaluation::CalculateMeanPageColor(const Page &page) {
  cv::Vec3f total_color(0.0f, 0.0f, 0.0f);
  for (std::shared_ptr<Piece> *current = page.GetFirstPiece(); current <= page.GetLastPiece(); current++) {
    total_color += (**current).DominatingColor();
  }
  float num_of_pieces = page.Size();
  return cv::Vec3f(total_color[0] / num_of_pieces, total_color[1] / num_of_pieces, total_color[2] / num_of_pieces);
}

float page_evaluation::CalculateVariance(const Page &page) {
  cv::Vec3f mean_color = page.MeanPageColor();
  double total_distance = 0.0;
  for (std::shared_ptr<Piece> *current = page.GetFirstPiece(); current <= page.GetLastPiece(); current++) {
    total_distance += ColorPiece::EuclideanDistance((**current).DominatingColor(), mean_color);
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
