#include <iostream>
#include <Mosaic/piece/LabIconPiece.hpp>
#include <Mosaic/Individual.hpp>
#include <Mosaic/IndividualGeneration.hpp>
#include <Mosaic/PopulationUtil.hpp>
#include <Mosaic/HallOfFame.hpp>
#include "Mosaic/JsonExport.hpp"

const int kNumOfPageBreaks = 3;
const int kNumOfPieces = 60;
const int kPopulation = 200;
const int kGenerations = 500;
const int kMaxAge = 50;

std::random_device rd;
std::mt19937 g(rd());


int main() {
  g.seed(0);
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

    population_util::PassThroughElites(population, temp_population, 10 * kPopulation / 100);
    population_util::MutateBest(population, temp_population, 60 * kPopulation / 100, g, i);
    population_util::FillShuffle(population, template_individual, kPopulation - population.size(), g, i);
    
    // update hall of fame with current population
    hall_of_fame.Update(population);
    
    population_util::PrintBest(population, 10);
  }

  population_util::PrintBest(population, 10);

  auto best = *population.begin();

  std::cout << best << std::endl;
  best.Print();
  best.Show();

  json_export::ExportIndividualToNDJSON(best, "best-individual.ndjson", json_export::LAB_ICON_PIECE);


  return 0;
}