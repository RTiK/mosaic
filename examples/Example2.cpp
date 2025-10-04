#include <iostream>
#include <Mosaic/piece/BgrIconPiece.hpp>
#include <Mosaic/Individual.hpp>
#include <Mosaic/IndividualGeneration.hpp>
#include <Mosaic/PopulationUtil.hpp>


const int kPopulation = 200;
const int kGenerations = 500;

std::random_device rd;
std::mt19937 g(rd());


int main() {
  g.seed(0);
  std::string dir_path = "/Users/rt/Desktop/icons";
  //Individual template_individual = individual_generation::ReadLabIcons(dir_path, 4, g);
  auto template_individual = individual_generation::GenerateIndividualGrayRandom(50, 2, g, 0);
  template_individual.Print();

  std::set<Individual> population{};
  population_util::FillShuffle(population, template_individual, kPopulation, g, 0);

  for (int i = 0; i < kGenerations; i++) {
    std::cout << " generation " << i << std::endl;

    std::set<Individual> temp_population{};
    temp_population.swap(population);

    population_util::PassThroughElites(population, temp_population, 10 * kPopulation / 100);
    population_util::MutateBest(population, temp_population, 60 * kPopulation / 100, g, i);
    population_util::FillShuffle(population, template_individual, kPopulation - population.size(), g, i);
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