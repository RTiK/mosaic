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
double evaluate_page(PageEdge page) {
    double totalDistance = 0.0;

    for (std::shared_ptr<Piece>* current = page.first; current <= page.second; current++) {
        unsigned int neighbours = calculateNeighbors(current, page.first, page.second);

        Piece* current_piece = current->get();

        current_piece->setNeighbors(neighbours);

        double pieceDistance = 0.0;

        if (neighbours & Neighbours::N) {
            pieceDistance += current_piece->getDistanceDelta76((current-4)->get());
        }
        if (neighbours & Neighbours::NE) {
            pieceDistance += current_piece->getDistanceDelta76((current-3)->get()) * DIAGONAL_WEIGHT;
        }
        if (neighbours & Neighbours::E) {
            pieceDistance += current_piece->getDistanceDelta76((current+1)->get());
        }
        if (neighbours & Neighbours::SE) {
            pieceDistance += current_piece->getDistanceDelta76((current+5)->get()) * DIAGONAL_WEIGHT;
        }
        if (neighbours & Neighbours::S) {
            pieceDistance += current_piece->getDistanceDelta76((current+4)->get());
        }
        if (neighbours & Neighbours::SW) {
            pieceDistance += current_piece->getDistanceDelta76((current+3)->get()) * DIAGONAL_WEIGHT;
        }
        if (neighbours & Neighbours::W) {
            pieceDistance += current_piece->getDistanceDelta76((current-1)->get());
        }
        if (neighbours & Neighbours::NW) {
            pieceDistance += current_piece->getDistanceDelta76((current-5)->get()) * DIAGONAL_WEIGHT;
        }

        current_piece->setCummultativeDistance(pieceDistance);

        totalDistance += current_piece->getNormalizedDistance();
    }

    return totalDistance;
}

unsigned int calculateNeighbors(std::shared_ptr<Piece>* current, std::shared_ptr<Piece>* first, std::shared_ptr<Piece>* last) {
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

double evaluate_genome(std::vector<std::shared_ptr<Piece>>& genome) {
    auto pages = splitGeneToPages(genome);
    double total_fitness = 0.0;
    for (std::shared_ptr<PageEdge>& page : pages) {
        total_fitness += evaluate_page(*page);
    }
    return total_fitness;
}