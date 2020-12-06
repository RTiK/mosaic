//
// Created by Artem Khatchatourov on 13.09.20.
//

#ifndef MOSAIC_INDIVIDUAL_H
#define MOSAIC_INDIVIDUAL_H

#include "Piece.h"

typedef std::pair<std::shared_ptr<Piece>*, std::shared_ptr<Piece>*> PageEdge;

class Individual {
    static const int ROWS_PER_PAGE = 6;
    static const int COLUMNS_PER_PAGE = 4;
    static const int MAX_PIECES_ON_PAGE = ROWS_PER_PAGE * COLUMNS_PER_PAGE;

    std::vector<std::shared_ptr<PageEdge>> pages;
    std::vector<std::shared_ptr<Piece>> individual;
public:
    static std::shared_ptr<Piece> PAGE_BREAK;

    explicit Individual(std::vector<std::shared_ptr<Piece>> gene);

    static unsigned int calculateNeighbors(std::shared_ptr<Piece>* current, std::shared_ptr<Piece>* first, std::shared_ptr<Piece>* last);

    // Evaluates the individual
    virtual double getFitness();

    static double evaluate_page(PageEdge page);

    static std::vector<std::shared_ptr<PageEdge>> splitGeneToPages(std::vector<std::shared_ptr<Piece>> gene);

};

#endif //MOSAIC_INDIVIDUAL_H
