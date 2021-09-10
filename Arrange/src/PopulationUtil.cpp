//
// Created by Artem Khatchatourov on 25.06.21.
//

#include "PopulationUtil.h"

void population_util::FillShuffle(std::set<Individual> &population, const Individual &template_individual, int n, std::mt19937 &g) {
  for (int i = n; i > 0; i--) {
    population.emplace(template_individual, g);  // call copy construct with shuffle arg
  }
}

void population_util::MutateBest(std::set<Individual> &population, const std::set<Individual> &old_pop, int n, std::mt19937 &g) {
  assert(!population.empty());
  // TODO this should be initialized globally
  std::uniform_int_distribution<> random_ind_length(0, population.begin()->Size() - 1);

  auto old_pop_iter = old_pop.begin();
  while (old_pop_iter != old_pop.end() && n > 0) {
    Individual ind = *old_pop_iter;
    ind.Swap(random_ind_length(g), random_ind_length(g));
    population.insert(ind);
    old_pop_iter++;
    n--;
  }
}

void population_util::PassThroughElites(std::set<Individual> &population, const std::set<Individual> &old_pop, int n, std::mt19937 &g) {
  auto old_pop_iter = old_pop.begin();
  while (old_pop_iter != old_pop.end() && n > 0) {
    population.insert(*old_pop_iter);
    old_pop_iter++;
    n--;
  }
}

void population_util::PrintBest(std::set<Individual> &population, int n) {
  auto current = population.begin();
  while (current != population.end() && n >= 0) {
    std::cout << current->GetFitness() << ", ";
    current++;
    n--;
  }
  std::cout << std::endl;
}