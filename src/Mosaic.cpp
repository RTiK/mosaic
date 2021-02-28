//
// Created by Artem Khatchatourov on 30.01.21.
//

#include <random>
#include "Piece.h"
#include "PageTools.h"
#include "Individual.h"

int NUM_OF_PAGE_BREAKS = 2;
int NUM_OF_PIECES = 40;
int POPULATION = 100;
int MAX_GENERATIONS = 1000;

std::random_device rd;
std::mt19937 g(rd());
std::uniform_int_distribution<> distribution(0, NUM_OF_PIECES - 1);

bool CompareIndividuals(Individual &a, Individual &b) {
  bool is_larger = a.fitness_ < b.fitness_;
  return is_larger;
}

void PrintBest(std::list<Individual> &population, int best_n) {
  auto iter = population.begin();
  for (int i = 0; i < best_n; i++) {
    std::cout << iter->fitness_ << std::endl;
    iter++;
  }
}

Individual GenerateIndividual(int length, int page_breaks) {
  Individual pieces;
  pieces.genome_.reserve(length + page_breaks);
  for (int i = 0; i < length; i++) {
    pieces.genome_.push_back(std::make_shared<Piece>(i));
  }

  for (int i = 0; i < page_breaks; i++) {
    pieces.genome_.push_back(PAGE_BREAK);
  }
  return pieces;
}

void PrintIndividual(Individual &individual) {
  for (std::shared_ptr<Piece> &p : individual.genome_) {
    if (p == PAGE_BREAK) {
      std::cout << "page break ";
    } else {
      std::cout << p->GetColor();
    }
  }
  std::cout << std::endl;
}

double MeanFitness(const std::list<Individual>& population) {
  double total_fitness = 0.0;
  for (const auto &individual : population) {
    total_fitness += individual.fitness_;
  }
  unsigned int num_of_elements = population.size();
  return total_fitness / num_of_elements;
}

int main() {
  //g.seed(0);

  Individual template_individual = GenerateIndividual(NUM_OF_PIECES, NUM_OF_PAGE_BREAKS);

  PrintIndividual(template_individual);

  std::list<Individual> population{};

  for (int i = 0; i < POPULATION; i++) {
    population.emplace_back(template_individual, g);
  }

  population.sort(CompareIndividuals);

  for (int i = 0; i < MAX_GENERATIONS; i++) {
    std::cout << "generation " << i << std::endl;

    std::list<Individual> temp_population{};

    temp_population.swap(population);

    // pass through elites
    for (int j = 0; j < POPULATION / 10; j++) {
      population.emplace_back(temp_population.front());
      auto ind = temp_population.front();
      ind.Swap(distribution(g), distribution(g));
      population.emplace_back(ind);
      temp_population.pop_front();
      // TODO allow some mutations
    }

    // mutate
    for (int j = 0; j < POPULATION / 2; j++) {
      auto temp_i = temp_population.front();
      temp_population.pop_front();
      temp_i.Swap(distribution(g), distribution(g));
      population.emplace_back(temp_i);
    }

    // fill remaining

    for (int j = population.size(); j < POPULATION; j++) {
      population.emplace_back(template_individual, g);
    }

    population.sort(CompareIndividuals);

    std::cout << "mean fitness: " << MeanFitness(population) << " top 10: ";
    auto current_ind = population.begin();
    unsigned int iter = 0;
    while (iter < 10) {
      std::cout << current_ind->fitness_ << ", ";
      current_ind++;
      iter++;
    }
    std::cout << std::endl;
  }

  PrintBest(population, 10);

  auto best = population.front();

  std::cout << best << std::endl;

  for (const std::shared_ptr<PageEdge> &page : SplitGeneToPages(best.genome_)) {
    ShowPage(*page, std::to_string(EvaluatePage(*page)));
  }

  return 0;
}