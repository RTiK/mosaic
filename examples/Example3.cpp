#include "Mosaic/JsonExport.hpp"
#include <Mosaic/piece/LabIconPiece.hpp>
#include <Mosaic/IndividualGeneration.hpp>
#include <Mosaic/PopulationUtil.hpp>
#include <Mosaic/HallOfFame.hpp>

const int kPopulation = 200;
const int kGenerations = 3000;
const int kMaxAge = 50;


std::random_device rd;
std::mt19937 g(rd());

int main() {
  g.seed(0);
  auto dir_path = "/Users/artem/Projects/mosaic/resources";
  auto template_individual = individual_generation::ReadRgbIcons(dir_path, 3, g, 0);
  std::set<Individual> population{};
  population_util::FillShuffle(population, template_individual, kPopulation, g, 0);

  HallOfFame hall_of_fame(10);

  // Set up JSON export config for hall of fame callback
  json_export::ExportConfig export_config;
  export_config.variance_weight = 0.4;
  export_config.icons_missing_weight = 0.1;
  export_config.diagonal_weight = 0.70711;
  export_config.page_width = 4;
  export_config.page_height = 6;
  export_config.piece_type = json_export::LAB_ICON_PIECE;

  // Set callback to export individuals as they enter the hall of fame
  std::string hof_export_file = "hall_of_fame_progress.ndjson";
  hall_of_fame.SetOnInsertCallback([&export_config, &hof_export_file](const Individual& ind, size_t rank) {
    json_export::ExportIndividualToNDJSON(ind, hof_export_file, export_config);
  });

  hall_of_fame.Update(population);

  for (int i = 0; i < kGenerations; i++) {
    std::cout << "generation " << i << std::endl;

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

  // Export final best individual
  json_export::ExportIndividualToNDJSON(best, "best-individual.ndjson", export_config);
  best.Print();
  best.Show();

  return 0;

}