//
// Created by Artem Khatchatourov on 13.09.20.
//

#include <valarray>
#include "Individual.h"
#include "Piece.h"

using namespace std;

std::shared_ptr<Piece> Individual::PAGE_BREAK = std::make_shared<Piece>(0, 0, 0);

double Individual::getFitness() {
    std::valarray<double> fitnesses(pages.size());
    std::transform(pages.begin()->get(), pages.end()->get(), std::begin(fitnesses), Individual::evaluate_page);
    return fitnesses.sum();
}

/**
* Calculates the fitness of page between first (inclusive) and last (inclusive).
* @param first First element of page array
* @param last Last element of page array
* @return Page fitness
*/
double Individual::evaluate_page(PageEdge page) {
    double totalDistance = 0.0;
    double diagonal_weight = 1/sqrt(2);

    for (std::shared_ptr<Piece>* current = page.first; current <= page.second; current++) {
        unsigned int neighbours = Individual::calculateNeighbors(current, page.first, page.second);

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
    //this->individual = gene;  // TODO copy?
    this->pages = splitGeneToPages(gene);
}

std::vector<std::shared_ptr<PageEdge>> Individual::splitGeneToPages(const std::vector<std::shared_ptr<Piece>> gene) {
    std::vector<std::shared_ptr<PageEdge>> gene_pages{};
    std::shared_ptr<PageEdge> currentPagePtr;

    for (auto* p = const_cast<std::shared_ptr<Piece>*>(&gene.front()); p <= const_cast<std::shared_ptr<Piece>*>(&gene.back()); p++) {
        if (currentPagePtr) {
            if (*p == Individual::PAGE_BREAK || p - currentPagePtr->first > MAX_PIECES_ON_PAGE) {
                // overwrite page termination with the previous piece ptr
                currentPagePtr.reset();
            } else {
                currentPagePtr->second = p;
            }
        } else {
            if (*p != Individual::PAGE_BREAK) {
                // terminate the page at the end of the gene in case it doesn't end with a page break
                currentPagePtr = std::make_shared<PageEdge>(p, const_cast<shared_ptr<Piece> *>(&gene.back()));
                gene_pages.emplace_back(currentPagePtr);
            }
        }
    }

    return gene_pages;
}
