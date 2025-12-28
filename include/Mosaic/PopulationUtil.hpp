#ifndef MOSAIC_SRC_POPULATIONUTIL_H_
#define MOSAIC_SRC_POPULATIONUTIL_H_

#include <set>
#include <random>
#include "Individual.hpp"

namespace population_util {

  void FillShuffle(std::set<Individual> &population, const Individual &template_individual, int n, std::mt19937 &g, int current_generation);

  void MutateBest(std::set<Individual> &population, const std::set<Individual> &old_pop, int n, std::mt19937 &g, int current_generation);

  void PassThroughElites(std::set<Individual> &population, const std::set<Individual> &old_pop, int n);

  void PrintBest(std::set<Individual> &population, int n);

  void FilterByAge(std::set<Individual> &population, int current_generation, int max_age);

}; // namespace population_util

#endif //MOSAIC_SRC_POPULATIONUTIL_H_
