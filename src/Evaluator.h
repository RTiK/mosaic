//
// Created by Artem Khatchatourov on 23.01.21.
//

#ifndef MOSAIC_EVALUATOR_H
#define MOSAIC_EVALUATOR_H

#include "PageTools.h"

static double DIAGONAL_WEIGHT = 1/sqrt(2);

double evaluate_page(PageEdge page);

unsigned int calculateNeighbors(std::shared_ptr<Piece>* current, std::shared_ptr<Piece>* first, std::shared_ptr<Piece>* last);

#endif //MOSAIC_EVALUATOR_H
