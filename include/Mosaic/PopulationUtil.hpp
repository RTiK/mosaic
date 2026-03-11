#ifndef MOSAIC_SRC_POPULATIONUTIL_H_
#define MOSAIC_SRC_POPULATIONUTIL_H_

#include <set>
#include <random>
#include "Individual.hpp"

namespace population_util {

  /**
   * Fills the population with n randomly permuted individuals based on the template individual.
   */
  void FillShuffle(std::set<Individual> &population, const Individual &template_individual, int n, std::mt19937 &g, int current_generation);

  /**
   * Performs pairwise piece swaps on the best n individuals assuming the population is sorted by fitness.
   */
  void MutateBest(std::set<Individual> &population, const std::set<Individual> &old_pop, int n, std::mt19937 &g, int current_generation);

  /**
   * Passes through the best n individuals assuming the population is sorted by fitness.
   */
  void PassThroughElites(std::set<Individual> &population, const std::set<Individual> &old_pop, int n);

  /**
   * Prints n best individuals to std::cout assuming the population is sorted by fitness.
   */
  void PrintBest(const std::set<Individual> &population, int n);

  /**
   * Removes individuals from the population that are older than max_age.
   */
  void FilterByAge(std::set<Individual> &population, int current_generation, int max_age);

}; // namespace population_util

#endif //MOSAIC_SRC_POPULATIONUTIL_H_
