#include <random>
#include <Mosaic/piece/LabPiece.hpp>
#include <Mosaic/Individual.hpp>
#include <Mosaic/HallOfFame.hpp>
#include <Mosaic/IndividualGeneration.hpp>
#include <Mosaic/PopulationUtil.hpp>

/**
 * This example creates a population of grayscale pieces and arranges them into pages.
 */

const int kPageBreaks = 2;
const int kNumOfPieces = 40;
const int kPopulation = 200;
const int kGenerations = 1000;
const int kMaxAge = 50;
const int kPercentageElites = 10;
const int kPercentageMutants = 60;

const FitnessWeights kFitnessWeights = {
  .variance_weight = 1.9,
  .missing_icons_weight = 0.4
};

std::random_device rd;
std::mt19937 g(rd());


int main() {
  // setting the seed to a fixed value will make the algorithm produce the same results on every run
  // g.seed(0);

  Individual template_individual = individual_generation::GenerateIndividualGrayRandom(kNumOfPieces, kPageBreaks, g, 0, kFitnessWeights);

  template_individual.Print();

  std::set<Individual> population{};
  population_util::FillShuffle(population, template_individual, kPopulation, g, 0);

  HallOfFame hall_of_fame(10);
  hall_of_fame.Update(population);

  for (int i = 0; i < kGenerations; i++) {
    std::cout << "generation " << i << std::endl;

    std::set<Individual> temp_population{};
    temp_population.swap(population);

    population_util::FilterByAge(temp_population, i, kMaxAge);

    population_util::PassThroughElites(population, temp_population, kPercentageElites * kPopulation / 100);

    population_util::MutateAndPassBest(population, temp_population, kPercentageMutants * kPopulation / 100, g, i);

    population_util::FillShuffle(population, template_individual, kPopulation - population.size(), g, i);

    // update hall of fame with current population
    hall_of_fame.Update(population);

    population_util::PrintBest(population, 10);
  }

  // Final hall of fame results
  std::cout << "\n=== FINAL HALL OF FAME ===" << std::endl;
  hall_of_fame.Print();

  auto best = hall_of_fame.GetBest();

  std::cout << "\n=== BEST INDIVIDUAL EVER ===" << std::endl;
  std::cout << best << std::endl;

  best.Print();
  best.Show();

  return 0;
}