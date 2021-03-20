//
// Created by Artem Khatchatourov on 14.03.21.
//

#include "Page.h"


Page::Page(std::shared_ptr<Piece> *first_piece, std::shared_ptr<Piece> *last_piece)
    : first_piece_{first_piece}, last_piece_{last_piece} {
  fitness_ = Evaluate(first_piece_, last_piece_);
}

double Page::Evaluate(std::shared_ptr<Piece> *first_piece, std::shared_ptr<Piece> *last_piece) {
  double total_distance = 0.0;

  for (std::shared_ptr<Piece> *current = first_piece; current <= last_piece; current++) {
    unsigned int neighbours = CalculateNeighbors(current, first_piece, last_piece);

    Piece *current_piece = current->get();

    current_piece->SetNeighbors(neighbours);

    double piece_distance = 0.0;

    if (neighbours & Neighbours::N) {
      piece_distance += current_piece->GetEuclideanDistance(**(current-4));
    }
    if (neighbours & Neighbours::NE) {
      piece_distance += current_piece->GetEuclideanDistance(**(current-3)) * diagonal_weight_;
    }
    if (neighbours & Neighbours::E) {
      piece_distance += current_piece->GetEuclideanDistance(**(current+1));
    }
    if (neighbours & Neighbours::SE) {
      piece_distance += current_piece->GetEuclideanDistance(**(current+5)) * diagonal_weight_;
    }
    if (neighbours & Neighbours::S) {
      piece_distance += current_piece->GetEuclideanDistance(**(current+4));
    }
    if (neighbours & Neighbours::SW) {
      piece_distance += current_piece->GetEuclideanDistance(**(current+3)) * diagonal_weight_;
    }
    if (neighbours & Neighbours::W) {
      piece_distance += current_piece->GetEuclideanDistance(**(current-1));
    }
    if (neighbours & Neighbours::NW) {
      piece_distance += current_piece->GetEuclideanDistance(**(current-5)) * diagonal_weight_;
    }

    current_piece->SetTotalDistance(piece_distance);

    total_distance += current_piece->GetNormalizedDistance();
  }

  return total_distance;
}


unsigned int Page::CalculateNeighbors(std::shared_ptr<Piece> *current,
                                      std::shared_ptr<Piece> *first,
                                      std::shared_ptr<Piece> *last) {
  auto delta_a = current - first;
  auto delta_b = last - current;

  unsigned int neighbours = Neighbours::ALL;

  if (delta_a % 4 == 0) {  // position on far left -> can't go west
    neighbours &= ~(Neighbours::NW | Neighbours::W | Neighbours::SW);
  } else if (delta_a % 4 == 3) {  // position on far right -> can't go east
    neighbours &= ~(Neighbours::SE | Neighbours::E | Neighbours::NE);
  }

  if (delta_a < 4) {  // can't go north
    neighbours &= ~(Neighbours::NW | Neighbours::N | Neighbours::NE);
  }

  if (delta_b < 3) {  // can't go immediate south
    neighbours &= ~(Neighbours::SW | Neighbours::S | Neighbours::SE);
  }

  if (delta_b == 3) {
    neighbours &= ~(Neighbours::S | Neighbours::SE);
  }

  if (delta_b == 4) {
    neighbours &= ~Neighbours::SE;
  }

  if (delta_b == 0) {
    neighbours &= ~(Neighbours::E | Neighbours::SE | Neighbours::S | Neighbours::SW);
  }

  return neighbours;
}
