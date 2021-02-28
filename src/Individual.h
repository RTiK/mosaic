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

  Individual(const Individual &ind);

  Individual(const Individual &ind, std::mt19937 g);

  std::vector<std::shared_ptr<Piece>> genome_;

  double fitness_ = MAXFLOAT;

  void Swap(unsigned int index_1, unsigned int index_2);

  friend bool operator<(const Individual &ind_1, const Individual &ind_2);

  friend std::ostream &operator<<(std::ostream &out, Individual &ind);
};

#endif //MOSAIC_INDIVIDUAL_H
