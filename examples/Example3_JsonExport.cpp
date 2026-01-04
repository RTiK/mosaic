#include <Mosaic/piece/BgrIconPiece.hpp>
#include <Mosaic/IndividualGeneration.hpp>
#include <Mosaic/PopulationUtil.hpp>
#include <Mosaic/HallOfFame.hpp>
#include <Mosaic/JsonExport.hpp>

const int kPopulation = 200;
const int kGenerations = 100;  // Reduced for demo
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
  hall_of_fame.Update(population);

  // Setup JSON export configuration
  json_export::ExportConfig export_config;
  export_config.diagonal_weight = 0.70711;
  export_config.page_width = 4;
  export_config.page_height = 6;
  export_config.variance_weight = 0.4;
  export_config.icons_missing_weight = 0.1;
  export_config.piece_type = json_export::BGR_ICON_PIECE;

  std::string export_file = "evolution_results.ndjson";

  // Export initial best individual
  auto best = *population.begin();
  json_export::ExportIndividualToNDJSON(best, export_file, export_config);
  std::cout << "Exported initial best individual (fitness: " << best.GetFitness() << ")" << std::endl;

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

    // Export best individual every 10 generations
    if ((i + 1) % 10 == 0) {
      auto current_best = *population.begin();
      json_export::ExportIndividualToNDJSON(current_best, export_file, export_config);
      std::cout << "Exported generation " << (i + 1) << " best (fitness: "
                << current_best.GetFitness() << ")" << std::endl;
    }

    // print hall of fame every 25 generations
    if ((i + 1) % 25 == 0) {
      std::cout << "\n--- Generation " << (i + 1) << " ---" << std::endl;
      hall_of_fame.Print();
      std::cout << std::endl;
    }
  }

  population_util::PrintBest(population, 10);

  // Final hall of fame results
  std::cout << "\n=== FINAL HALL OF FAME ===" << std::endl;
  hall_of_fame.Print();

  // Export final best individual
  auto final_best = hall_of_fame.GetBestIndividual();
  json_export::ExportIndividualToNDJSON(final_best, export_file, export_config);

  std::cout << "\n=== BEST INDIVIDUAL EVER ===" << std::endl;
  std::cout << final_best << std::endl;
  std::cout << "\nExported to: " << export_file << std::endl;

  final_best.Print();
  final_best.Show();

  return 0;
}
