//
// Created by Artem Khatchatourov on 23.01.21.
//

#include "Evaluator.h"

/**
* Calculates the fitness of page between first (inclusive) and last (inclusive).
* @param first First element of page array
* @param last Last element of page array
* @return Page fitness
*/
double EvaluatePage(PageEdge page) {
  double total_distance = 0.0;

  for (std::shared_ptr<Piece> *current = page.first; current <= page.second; current++) {
    unsigned int neighbours = CalculateNeighbors(current, page.first, page.second);

    Piece *current_piece = current->get();

    current_piece->SetNeighbors(neighbours);

    double piece_distance = 0.0;

    if (neighbours & Neighbours::N) {
      piece_distance += GetDistance(**current, **(current-4));
    }
    if (neighbours & Neighbours::NE) {
      piece_distance += GetDistance(**current, **(current-3)) * DIAGONAL_WEIGHT;
    }
    if (neighbours & Neighbours::E) {
      piece_distance += GetDistance(**current, **(current+1));
    }
    if (neighbours & Neighbours::SE) {
      piece_distance += GetDistance(**current, **(current+5)) * DIAGONAL_WEIGHT;
    }
    if (neighbours & Neighbours::S) {
      piece_distance += GetDistance(**current, **(current+4));
    }
    if (neighbours & Neighbours::SW) {
      piece_distance += GetDistance(**current, **(current+3)) * DIAGONAL_WEIGHT;
    }
    if (neighbours & Neighbours::W) {
      piece_distance += GetDistance(**current, **(current-1));
    }
    if (neighbours & Neighbours::NW) {
      piece_distance += GetDistance(**current, **(current-5)) * DIAGONAL_WEIGHT;
    }

    current_piece->SetTotalDistance(piece_distance);

    total_distance += current_piece->GetNormalizedDistance();
  }

  return total_distance;
}

unsigned int CalculateNeighbors(std::shared_ptr<Piece> *current,
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

double EvaluateGenome(std::vector<std::shared_ptr<Piece>> &genome) {
  auto pages = SplitGeneToPages(genome);
  double total_fitness = 0.0;
  for (std::shared_ptr<PageEdge> &page : pages) {
    total_fitness += EvaluatePage(*page);
  }
  return total_fitness;
}