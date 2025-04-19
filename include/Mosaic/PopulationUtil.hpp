//
// Created by Artem Khatchatourov on 25.06.21.
//

#ifndef MOSAIC_SRC_POPULATIONUTIL_H_
#define MOSAIC_SRC_POPULATIONUTIL_H_

#include "Individual.hpp"

namespace population_util {

  void FillShuffle(std::set<Individual> &population, const Individual &template_individual, int n, std::mt19937 &g);

  void MutateBest(std::set<Individual> &population, const std::set<Individual> &old_pop, int n, std::mt19937 &g);

  void PassThroughElites(std::set<Individual> &population, const std::set<Individual> &old_pop, int n, std::mt19937 &g);

  void PrintBest(std::set<Individual> &population, int n);

};

#endif //MOSAIC_SRC_POPULATIONUTIL_H_
