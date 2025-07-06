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
    ind.Swap(random_ind_length(g), random_ind_length(g));
    population.insert(ind);
    old_pop_iter++;
    n--;
  }
}

void population_util::PassThroughElites(std::set<Individual> &population, const std::set<Individual> &old_pop, int n, std::mt19937 &g, int max_age) {
  auto old_pop_iter = old_pop.begin();
  while (old_pop_iter != old_pop.end() && n > 0) {
    Individual ind = *old_pop_iter;
    ind.IncrementAge();
    
    // If max_age is -1 (default), skip age filtering; otherwise filter by age
    if (max_age == -1 || ind.GetAge() <= max_age) {
      population.insert(ind);
      n--;
    }
    old_pop_iter++;
  }
}

void population_util::PrintBest(std::set<Individual> &population, int n) {
  auto current = population.begin();
  while (current != population.end() && n >= 0) {
    std::cout << std::format("{}, {}", current->GetFitness(), current->GetAge()) << " | ";
    current++;
    n--;
  }
  std::cout << std::endl;
}

