//
// Created by Artem Khatchatourov on 13.09.20.
//

#ifndef MOSAIC_INDIVIDUAL_H
#define MOSAIC_INDIVIDUAL_H

#include "Piece.h"


class Individual {
    const int ROWS_PER_PAGE = 6;
    const int COLUMNS_PER_PAGE = 4;
    const int ICONS_ON_PAGE = ROWS_PER_PAGE * COLUMNS_PER_PAGE;

    const std::shared_ptr<Piece> PAGE_BREAK = std::make_shared<Piece>(0, 0, 0);

    std::vector<std::shared_ptr<Piece>> individual;

public:
    explicit Individual(std::vector<std::shared_ptr<Piece>> gene);

    static unsigned int calculateNeighbors(std::shared_ptr<Piece>* current, std::shared_ptr<Piece>* first, std::shared_ptr<Piece>* last);

    // Evaluates the individual
    double evaluate();

    double evaluate_page(std::shared_ptr<Piece>* first, std::shared_ptr<Piece>* last);

};

#endif //MOSAIC_INDIVIDUAL_H
