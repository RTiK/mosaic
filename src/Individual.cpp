//
// Created by Artem Khatchatourov on 13.09.20.
//

#include "Individual.h"

using namespace std;

double Individual::evaluate() {
    Piece **current_first = &individual.front();
    Piece **current_last = &individual.back();
    Piece **current = &individual.front();

    auto pbptr = &PAGE_BREAK;

    while (current <= &individual.back()) {
        // find first non page break element
        while (*current == pbptr) {
            current++;
        }
        current_first = current;

        int elems_on_page = 0;

        // find first page break element and store the previous element
        while (*current != pbptr && current_last <= &individual.back() && elems_on_page < ICONS_ON_PAGE) {
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
double Individual::evaluate_page(Piece** first, Piece** last) {
    double distance = 0.0;
    double diagonal_weight = 1/sqrt(2);

    for (Piece** current = first; current <= last; current++) {
        unsigned int neighbours = Individual::calculateNeighbors(current, first, last);

        if ((neighbours & Neighbours::N) > 0) {
            distance += (*current)->getDistanceDelta76(*current-4);
            //distance += deltaE76(*current, *(current-4));
        }
        if ((neighbours & Neighbours::NE) > 0) {
            distance += (*current)->getDistanceDelta76(*current-3) * diagonal_weight;
            //distance += deltaE76(*current, *(current-3)) * diagonal_weight;
        }
        if ((neighbours & Neighbours::E) > 0) {
            distance += (*current)->getDistanceDelta76(*current+1);
            //distance += deltaE76(*current, *(current+1));
        }
        if ((neighbours & Neighbours::SE) > 0) {
            distance += (*current)->getDistanceDelta76(*current+5) * diagonal_weight;
            //distance += deltaE76(*current, *(current+5)) * diagonal_weight;
        }
        if ((neighbours & Neighbours::S) > 0) {
            distance += (*current)->getDistanceDelta76(*current+4);
            //distance += deltaE76(*current, *(current+4));
        }
        if ((neighbours & Neighbours::SW) > 0) {
            distance += (*current)->getDistanceDelta76(*current-3) * diagonal_weight;
            //distance += deltaE76(*current, *(current-3)) * diagonal_weight;
        }
        if ((neighbours & Neighbours::W) > 0) {
            distance += (*current)->getDistanceDelta76(*current-1);
            //distance += deltaE76(*current, *(current-1));
        }
        if ((neighbours & Neighbours::NW) > 0) {
            distance += (*current)->getDistanceDelta76(*current-5) * diagonal_weight;
            //distance += deltaE76(*current, *(current-5)) * diagonal_weight;
        }
    }

    return distance;
}

unsigned int Individual::calculateNeighbors(Piece** current, Piece** first, Piece** last) {
    auto delta_a = current - first;
    auto delta_b = last - current;

    unsigned int neighbours = Neighbours::N | Neighbours::NE | Neighbours::E | Neighbours::SE
                            | Neighbours::S | Neighbours::SW | Neighbours::W | Neighbours::NW;

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

Individual::Individual(std::vector<Piece *> gene) {
    this->individual = gene;  // TODO copy?
}
