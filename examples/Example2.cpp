#include <iostream>
#include <Mosaic/piece/LabIconPiece.hpp>
#include <Mosaic/Individual.hpp>
#include <Mosaic/IndividualGeneration.hpp>
#include <Mosaic/PopulationUtil.hpp>
#include <Mosaic/HallOfFame.hpp>
#include "Mosaic/JsonExport.hpp"

/**
 * This example creates a population of color pieces and arranges them into pages.
 * Additionally, this example logs any changes to the hall of fame into a NDJSON file with `JsonExport`. This file can 
 * be analyzed in a Jupyter notebook found in the directory `visualization`.
 */

const int kNumOfPageBreaks = 3;
const int kNumOfPieces = 60;
const int kPopulation = 200;
const int kGenerations = 1000;
const int kMaxAge = 50;

std::random_device rd;
std::mt19937 g(rd());


int main() {
  // setting the seed to a fixed value will make the algorithm produce the same results on every run
  // g.seed(0);
  
  Individual template_individual = individual_generation::GenerateIndividualLabRandom(kNumOfPieces, kNumOfPageBreaks, g, 0);
  
  template_individual.Print();

  std::set<Individual> population{};
  population_util::FillShuffle(population, template_individual, kPopulation, g, 0);

  HallOfFame hall_of_fame(10);

  // Set callback to export individuals as they enter the hall of fame
  std::string hof_export_file = "hall_of_fame_progress.ndjson";
  hall_of_fame.SetOnInsertCallback([&hof_export_file](const Individual& ind, size_t rank) {
    json_export::ExportIndividualToNDJSON(ind, hof_export_file, json_export::LAB_PIECE);
  });

  hall_of_fame.Update(population);

  for (int i = 0; i < kGenerations; i++) {
    std::cout << " generation " << i << std::endl;

    std::set<Individual> temp_population{};
    temp_population.swap(population);

    // remove elites that have been around for too long
    population_util::FilterByAge(temp_population, i, kMaxAge);

    // pass through elites (first 10%) with age filtering
    population_util::PassThroughElites(population, temp_population, 10 * kPopulation / 100);
    
    // mutate (another 60%) from temp population
    population_util::MutateBest(population, temp_population, 60 * kPopulation / 100, g, i);
    
    // fill remaining (~30%)
    population_util::FillShuffle(population, template_individual, kPopulation - population.size(), g, i);
    
    // update hall of fame with current population
    hall_of_fame.Update(population);
    
    population_util::PrintBest(population, 10);
  }

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