//
// Created by Artem Khatchatourov on 13.09.20.
//

#ifndef MOSAIC_INDIVIDUAL_H
#define MOSAIC_INDIVIDUAL_H

#include "Piece.h"


enum Neighbours: unsigned int {
    N   = 1<<7,
    NE  = 1<<6,
    E   = 1<<5,
    SE  = 1<<4,
    S   = 1<<3,
    SW  = 1<<2,
    W   = 1<<1,
    NW  = 1<<0,
    ALL = N | NE | E | SE | S | SW | W | NW,
    NONE= 0
};

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

private:
    double evaluate_page(std::shared_ptr<Piece>* first, std::shared_ptr<Piece>* last);

};

#endif //MOSAIC_INDIVIDUAL_H
