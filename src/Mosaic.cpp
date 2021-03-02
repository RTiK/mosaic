//
// Created by Artem Khatchatourov on 30.01.21.
//

#include <random>
#include "Piece.h"
#include "PageTools.h"
#include "Individual.h"
#include "FileLogger.h"

// TODO piece inheritance, RGBPiece

int NUM_OF_PAGE_BREAKS = 2;
int NUM_OF_PIECES = 40;
int POPULATION = 100;
int MAX_GENERATIONS = 2000;
std::string LOGFILE = "/Users/rt/Desktop/mosaic_gray.csv";

std::random_device rd;
std::mt19937 g(rd());
std::uniform_int_distribution<> random_ind_length(0, NUM_OF_PIECES - 1);
std::uniform_int_distribution<> random_color_value(0, 100);


void PrintBest(std::set<Individual> &population, int best_n) {
  auto iter = population.begin();
  for (int i = 0; i < best_n; i++) {
    std::cout << iter->fitness_ << std::endl;
    iter++;
  }
}

Individual GenerateIndividualGray(int length, int page_breaks) {
  Individual ind;
  ind.genome_.reserve(length + page_breaks);
  for (int i = 0; i < length; i++) {
    ind.genome_.push_back(std::make_shared<Piece>(i));
  }

  for (int i = 0; i < page_breaks; i++) {
    ind.genome_.push_back(PAGE_BREAK);
  }
  return ind;
}

Individual GenerateIndividualGrayRandom(int length, int page_breaks) {
  Individual ind;
  ind.genome_.reserve(length + page_breaks);
  for (int i = 0; i < length; i++) {
    ind.genome_.push_back(std::make_shared<Piece>(random_color_value(g)));
  }

  for (int i = 0; i < page_breaks; i++) {
    ind.genome_.push_back(PAGE_BREAK);
  }
  return ind;
}

Individual GenerateIndividualRgbRandom(int length, int page_breaks) {
  Individual ind;
  ind.genome_.reserve(length + page_breaks);
  for (int i = 0; i < length; i++) {
    ind.genome_.push_back(std::make_shared<Piece>(random_color_value(g),
                                                  random_color_value(g),
                                                  random_color_value(g)));
  }

  for (int i = 0; i < page_breaks; i++) {
    ind.genome_.push_back(PAGE_BREAK);
  }
  return ind;
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

double MeanFitness(const std::set<Individual>& population) {
  double total_fitness = 0.0;
  for (const auto &individual : population) {
    total_fitness += individual.fitness_;
  }
  unsigned int num_of_elements = population.size();
  return total_fitness / num_of_elements;
}

void PassThroughElites(std::set<Individual> &new_pop, const std::set<Individual> &old_pop, double percent) {
  auto old_pop_iter = old_pop.begin();
  do {
    new_pop.insert(*old_pop_iter);
    old_pop_iter++;
  } while (new_pop.size() < percent * (POPULATION / 100.0) && old_pop_iter != old_pop.end());
}

void MutateBest(std::set<Individual> &new_pop, const std::set<Individual> &old_pop, double percent) {
  auto old_pop_iter = old_pop.begin();
  do {
    Individual ind = *old_pop_iter;
    ind.Swap(random_ind_length(g), random_ind_length(g));
    new_pop.insert(ind);
    old_pop_iter++;
  } while (new_pop.size() < percent * (POPULATION / 100.0) && old_pop_iter != old_pop.end());
}

void FillShuffle(std::set<Individual> &new_pop, const Individual &template_individual, double percent) {
  for (int i = 0; i < percent; i++) {
    new_pop.emplace(template_individual, g);  // call copy construct with shuffle arg
  }
}

int main() {
  //g.seed(0);
  FileLogger logger(LOGFILE);
  Individual template_individual = GenerateIndividualGrayRandom(NUM_OF_PIECES, NUM_OF_PAGE_BREAKS);
      //GenerateIndividualRgbRandom(NUM_OF_PIECES, NUM_OF_PAGE_BREAKS);
      //GenerateIndividualGray(NUM_OF_PIECES, NUM_OF_PAGE_BREAKS);
  PrintIndividual(template_individual);

  std::set<Individual> population{};
  FillShuffle(population, template_individual, POPULATION);

  for (int i = 0; i < MAX_GENERATIONS; i++) {
    std::cout << "generation " << i << std::endl;

    std::set<Individual> temp_population{};
    temp_population.swap(population);

    // pass through elites (first 10%)
    PassThroughElites(population, temp_population, 10);

    // mutate (another 60%)
    MutateBest(population, temp_population, 60);

    // fill remaining (~30%)
    FillShuffle(population, template_individual, population.size() / (POPULATION / 100));

    logger.Log(population);

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

  auto best = *population.begin();

  std::cout << best << std::endl;

  for (const std::shared_ptr<PageEdge> &page : SplitGeneToPages(best.genome_)) {
    ShowPage(*page, std::to_string(EvaluatePage(*page)));
  }

  return 0;
}