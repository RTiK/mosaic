#include "Mosaic/PopulationUtil.hpp"

void population_util::FillShuffle(std::set<Individual> &population, const Individual &template_individual, int n, std::mt19937 &g, int current_generation) {
  for (int i = n; i > 0; i--) {
    population.emplace(template_individual, g, current_generation);  // call copy construct with shuffle arg
  }
}

void population_util::MutateBest(std::set<Individual> &population, const std::set<Individual> &old_pop, int n, std::mt19937 &g, int current_generation) {
  assert(!population.empty());
  // TODO this should be initialized globally
  std::uniform_int_distribution<> random_ind_length(0, population.begin()->Size() - 1);

  auto old_pop_iter = old_pop.begin();
  while (old_pop_iter != old_pop.end() && n > 0) {
    Individual ind = *old_pop_iter;
    ind.Swap(random_ind_length(g), random_ind_length(g), current_generation);
    population.insert(ind);
    old_pop_iter++;
    n--;
  }
}

void population_util::PassThroughElites(std::set<Individual> &population, const std::set<Individual> &old_pop, int n) {
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
    std::cout << std::format("{}, {}", current->GetFitness(), current->GetBirthGeneration()) << " | ";
    current++;
    n--;
  }
  std::cout << std::endl;
}

void population_util::FilterByAge(std::set<Individual> &population, int current_generation, int max_age) {
    auto it = population.begin();
    while (it != population.end()) {
        int age = current_generation - it->GetBirthGeneration();
        if (age > max_age) {
            it = population.erase(it);  // erase returns iterator to next element
        } else {
            ++it;
        }
    }
}

