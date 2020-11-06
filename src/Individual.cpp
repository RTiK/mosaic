//
// Created by Artem Khatchatourov on 13.09.20.
//

#include "Individual.h"
#include "Piece.h"

using namespace std;

double Individual::evaluate() {
    std::shared_ptr<Piece>* current_first = &individual.front();
    std::shared_ptr<Piece>* current_last = &individual.back();
    std::shared_ptr<Piece>* current = &individual.front();

    while (current <= &individual.back()) {
        // find first non page break element
        while (*current == PAGE_BREAK) {
            current++;
        }
        current_first = current;

        int elems_on_page = 0;

        // find first page break element and store the previous element
        while (*current != PAGE_BREAK && current_last <= &individual.back() && elems_on_page < ICONS_ON_PAGE) {
            current++;
            elems_on_page++;
        }
        current_last = current-1;

        double page_fitness = evaluate_page(current_first, current_last);
    }
    return 0;
}

/**
* Calculates the fitness of page between first (inclusive) and last (inclusive).
* @param first First element of page array
* @param last Last element of page array
* @return Page fitness
*/
double Individual::evaluate_page(std::shared_ptr<Piece>* first, std::shared_ptr<Piece>* last) {
    double totalDistance = 0.0;
    double diagonal_weight = 1/sqrt(2);

    for (std::shared_ptr<Piece>* current = first; current <= last; current++) {
        unsigned int neighbours = Individual::calculateNeighbors(current, first, last);

        Piece* current_piece = current->get();

        current_piece->setNeighbors(neighbours);

        double pieceDistance = 0.0;

        if (neighbours & Neighbours::N) {
            pieceDistance += current_piece->getDistanceDelta76((current-4)->get());
        }
        if (neighbours & Neighbours::NE) {
            pieceDistance += current_piece->getDistanceDelta76((current-3)->get()) * diagonal_weight;
        }
        if (neighbours & Neighbours::E) {
            pieceDistance += current_piece->getDistanceDelta76((current+1)->get());
        }
        if (neighbours & Neighbours::SE) {
            pieceDistance += current_piece->getDistanceDelta76((current+5)->get()) * diagonal_weight;
        }
        if (neighbours & Neighbours::S) {
            pieceDistance += current_piece->getDistanceDelta76((current+4)->get());
        }
        if (neighbours & Neighbours::SW) {
            pieceDistance += current_piece->getDistanceDelta76((current+3)->get()) * diagonal_weight;
        }
        if (neighbours & Neighbours::W) {
            pieceDistance += current_piece->getDistanceDelta76((current-1)->get());
        }
        if (neighbours & Neighbours::NW) {
            pieceDistance += current_piece->getDistanceDelta76((current-5)->get()) * diagonal_weight;
        }

        current_piece->setCummultativeDistance(pieceDistance);

        totalDistance += current_piece->getNormalizedDistance();
    }

    return totalDistance;
}

unsigned int Individual::calculateNeighbors(std::shared_ptr<Piece>* current, std::shared_ptr<Piece>* first, std::shared_ptr<Piece>* last) {
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

Individual::Individual(std::vector<std::shared_ptr<Piece>> gene) {
    this->individual = gene;  // TODO copy?
}
