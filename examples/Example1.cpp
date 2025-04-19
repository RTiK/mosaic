
//
// Created by Artem Khatchatourov on 30.01.21.
//

#include <random>
#include <Mosaic/piece/LabPiece.hpp>
#include <Mosaic/Individual.hpp>
#include <Mosaic/FileLogger.hpp>
#include <Mosaic/IndividualGeneration.hpp>
#include <Mosaic/PopulationUtil.hpp>

const int kNumOfPageBreaks = 2;
const int kNumOfPieces = 40;
const int kPopulation = 200;
const int kGenerations = 1000;


std::random_device rd;
std::mt19937 g(rd());


int main() {
  g.seed(0);
  Individual template_individual = individual_generation::GenerateIndividualLabRandom(kNumOfPieces, kNumOfPageBreaks, g);

  template_individual.Print();

  std::set<Individual> population{};
  population_util::FillShuffle(population, template_individual, kPopulation, g);

  for (int i = 0; i < kGenerations; i++) {
    std::cout << "generation " << i << std::endl;

    std::set<Individual> temp_population{};
    temp_population.swap(population);

    // pass through elites (first 10%)
    population_util::PassThroughElites(population, temp_population, 10 * kPopulation / 100, g);

    // mutate (another 60%)
    population_util::MutateBest(population, temp_population, 60 * kPopulation / 100, g);

    // fill remaining (~30%)
    population_util::FillShuffle(population, template_individual, kPopulation - population.size(), g);

    population_util::PrintBest(population, 10);

    auto best = *population.begin();
    best.Print();
  }

  population_util::PrintBest(population, 10);

  auto best = *population.begin();

  std::cout << best << std::endl;
  best.Print();
  best.Show();

  return 0;
}