#include <random>
#include <Mosaic/piece/LabPiece.hpp>
#include <Mosaic/Individual.hpp>
#include <Mosaic/FileLogger.hpp>
#include <Mosaic/HallOfFame.hpp>
#include <Mosaic/IndividualGeneration.hpp>
#include <Mosaic/PopulationUtil.hpp>

const int kNumOfPageBreaks = 2;
const int kNumOfPieces = 40;
const int kPopulation = 200;
const int kGenerations = 1000;
const int kMaxAge = 50;


std::random_device rd;
std::mt19937 g(rd());


int main() {
  g.seed(0);
  Individual template_individual = individual_generation::GenerateIndividualLabRandom(kNumOfPieces, kNumOfPageBreaks, g);

  template_individual.Print();

  std::set<Individual> population{};
  population_util::FillShuffle(population, template_individual, kPopulation, g, 0);

  HallOfFame hall_of_fame(10);
  hall_of_fame.Update(population);

  for (int i = 0; i < kGenerations; i++) {
    std::cout << "generation " << i << std::endl;

    std::set<Individual> temp_population{};
    temp_population.swap(population);

    // pass through elites (first 10%) with age filtering
    population_util::PassThroughElites(population, temp_population, 10 * kPopulation / 100, g);

    // mutate (another 60%) from temp population (age increment handled in PassThroughElites)
    population_util::MutateBest(population, temp_population, 60 * kPopulation / 100, g, i);

    // fill remaining (~30%)
    population_util::FillShuffle(population, template_individual, kPopulation - population.size(), g, i);

    // update hall of fame with current population
    hall_of_fame.Update(population);

    population_util::PrintBest(population, 10);

    //auto best = *population.begin();
    //best.Print();

    // print hall of fame every 100 generations
    if ((i + 1) % 100 == 0) {
      std::cout << "\n--- Generation " << (i + 1) << " ---" << std::endl;
      hall_of_fame.Print();
      std::cout << std::endl;
    }
  }

  population_util::PrintBest(population, 10);

  // Final hall of fame results
  std::cout << "\n=== FINAL HALL OF FAME ===" << std::endl;
  hall_of_fame.Print();

  auto best = hall_of_fame.GetBestIndividual();

  std::cout << "\n=== BEST INDIVIDUAL EVER ===" << std::endl;
  std::cout << best << std::endl;
  best.Print();
  best.Show();

  return 0;
}