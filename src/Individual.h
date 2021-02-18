//
// Created by Artem Khatchatourov on 15.02.21.
//

#include <vector>
#include <random>
#include "Piece.h"
#include "PageTools.h"
#include "Evaluator.h"

#ifndef MOSAIC_INDIVIDUAL_H
#define MOSAIC_INDIVIDUAL_H

class Individual {

public:
    Individual();

    Individual(const Individual& individual);

    Individual(const Individual &individual, std::mt19937 g);

    std::vector<std::shared_ptr<Piece>> genome;

    double fitness = MAXFLOAT;

    std::string to_string();

    void swap(unsigned int index1, unsigned int index2);
};

#endif //MOSAIC_INDIVIDUAL_H
