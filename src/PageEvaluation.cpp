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
      piece_distance += current_piece->GetEuclideanDistance(**(current - 4));
    }
    if (neighbours & Neighbours::NE) {
      piece_distance += current_piece->GetEuclideanDistance(**(current - 3)) * diagonal_weight;
    }
    if (neighbours & Neighbours::E) {
      piece_distance += current_piece->GetEuclideanDistance(**(current + 1));
    }
    if (neighbours & Neighbours::SE) {
      piece_distance += current_piece->GetEuclideanDistance(**(current + 5)) * diagonal_weight;
    }
    if (neighbours & Neighbours::S) {
      piece_distance += current_piece->GetEuclideanDistance(**(current + 4));
    }
    if (neighbours & Neighbours::SW) {
      piece_distance += current_piece->GetEuclideanDistance(**(current + 3)) * diagonal_weight;
    }
    if (neighbours & Neighbours::W) {
      piece_distance += current_piece->GetEuclideanDistance(**(current - 1));
    }
    if (neighbours & Neighbours::NW) {
      piece_distance += current_piece->GetEuclideanDistance(**(current - 5)) * diagonal_weight;
    }

    unsigned int num_of_neighbors = neighbours > 0 ? std::bitset<8>(neighbours).count() : 1;
    total_fitness += piece_distance / num_of_neighbors;
  }
  return total_fitness;
}

ColorT page_evaluation::CalculateMeanPageColor(const Page &page) {
  ColorT total_color = ColorT(0, 0, 0);
  for (std::shared_ptr<Piece> *current = page.GetFirstPiece(); current <= page.GetLastPiece(); current++) {
    total_color += (**current).GetColor();
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
    double euclidean = (**current).GetEuclideanDistance(mean_color);
    // euclidean distance is the square root of the mean squared error, we need to square it again
    //total_distance += pow(euclidean, 2) / mean_color.channels;
    total_distance += euclidean / mean_color.channels;
  }
  double num_of_pieces = 1.0 + double (page.GetLastPiece() - page.GetFirstPiece());
  return total_distance / num_of_pieces;
}

double page_evaluation::CalculateMissingIcons(const Page &page) {
  double icons_on_page = 1.0 + double (page.GetLastPiece() - page.GetFirstPiece());
  return 24.0 - icons_on_page;
}

unsigned int page_evaluation::CalculateNeighbors(unsigned int piece_index, unsigned int total_pieces) {
  unsigned int piece_index_end = total_pieces - 1 - piece_index;
  unsigned int neighbours = Neighbours::ALL;

  if (piece_index % 4 == 0) {  // position on far left -> can't go west
    neighbours &= ~(Neighbours::NW | Neighbours::W | Neighbours::SW);
  } else if (piece_index % 4 == 3) {  // position on far right -> can't go east
    neighbours &= ~(Neighbours::SE | Neighbours::E | Neighbours::NE);
  }

  if (piece_index < 4) {  // can't go north
    neighbours &= ~(Neighbours::NW | Neighbours::N | Neighbours::NE);
  }

  if (piece_index_end < 3) {  // can't go south
    neighbours &= ~(Neighbours::SW | Neighbours::S | Neighbours::SE);
  }

  if (piece_index_end == 3) {
    neighbours &= ~(Neighbours::S | Neighbours::SE);
  }

  if (piece_index_end == 4) {
    neighbours &= ~Neighbours::SE;
  }

  if (piece_index_end == 0) {
    neighbours &= ~(Neighbours::E | Neighbours::SE | Neighbours::S | Neighbours::SW);
  }

  return neighbours;
}
