//
// Created by Artem Khatchatourov on 22.04.21.
//

#include "PageEvaluation.h"

double page_evaluation::CalculatePageDistances(const Page &page, double diagonal_weight) {
  unsigned int total_pieces = page.Size();

  double total_fitness = 0.0;

  for (std::shared_ptr<Piece> *current = page.GetFirstPiece(); current <= page.GetLastPiece(); current++) {
    unsigned int piece_index = current - page.GetFirstPiece();
    unsigned char neighbours = page_evaluation::CalculateNeighbors(piece_index, total_pieces);

    Piece *current_piece = current->get();

    double piece_distance = 0.0;

    if (neighbours & Neighbours::N) {
      piece_distance += current_piece->GetEuclideanDistance(**(current - kWidth));
    }
    if (neighbours & Neighbours::NE) {
      piece_distance += current_piece->GetEuclideanDistance(**(current - (kWidth - 1))) * diagonal_weight;
    }
    if (neighbours & Neighbours::E) {
      piece_distance += current_piece->GetEuclideanDistance(**(current + 1));
    }
    if (neighbours & Neighbours::SE) {
      piece_distance += current_piece->GetEuclideanDistance(**(current + (kWidth + 1))) * diagonal_weight;
    }
    if (neighbours & Neighbours::S) {
      piece_distance += current_piece->GetEuclideanDistance(**(current + kWidth));
    }
    if (neighbours & Neighbours::SW) {
      piece_distance += current_piece->GetEuclideanDistance(**(current + (kWidth - 1))) * diagonal_weight;
    }
    if (neighbours & Neighbours::W) {
      piece_distance += current_piece->GetEuclideanDistance(**(current - 1));
    }
    if (neighbours & Neighbours::NW) {
      piece_distance += current_piece->GetEuclideanDistance(**(current - (kWidth + 1))) * diagonal_weight;
    }

    if (neighbours > 0) {
      unsigned int num_of_neighbors = std::bitset<8>(neighbours).count();
      total_fitness += piece_distance / num_of_neighbors;
    } else {
      total_fitness += DBL_MAX;  // having no neighbors means that we have a single piece on page and we ant to avoid that
    }

  }
  return total_fitness;
}

ColorT page_evaluation::CalculateMeanPageColor(const Page &page) {
  ColorT total_color = ColorT(0, 0, 0);
  for (std::shared_ptr<Piece> *current = page.GetFirstPiece(); current <= page.GetLastPiece(); current++) {
    total_color += (**current).GetInternalColor();
  }
  float num_of_pieces = 1.0f + float (page.GetLastPiece() - page.GetFirstPiece());
  return ColorT(total_color[0] / num_of_pieces,
                total_color[1] / num_of_pieces,
                total_color[2] / num_of_pieces);
}

double page_evaluation::CalculateColorVariance(const Page &page) {
  ColorT mean_color = CalculateMeanPageColor(page);
  double total_distance = 0.0;
  for (std::shared_ptr<Piece> *current = page.GetFirstPiece(); current <= page.GetLastPiece(); current++) {
    total_distance += (**current).GetEuclideanDistance(mean_color);
  }
  double num_of_pieces = 1.0 + std::distance(page.GetFirstPiece(), page.GetLastPiece());
  return total_distance;
}

unsigned int page_evaluation::CalculateNeighbors(unsigned int piece_index, unsigned int total_pieces) {
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
