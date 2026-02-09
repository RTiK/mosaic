#include <Mosaic/piece/LabIconPiece.hpp>
#include <Mosaic/IndividualGeneration.hpp>
#include <Mosaic/JsonExport.hpp>
#include <iostream>

std::random_device rd;
std::mt19937 g(rd());

int main() {
  g.seed(42);

  // Create a small test individual with just a few icons
  auto dir_path = "/Users/artem/Projects/mosaic/resources";
  auto template_individual = individual_generation::ReadRgbIcons(dir_path, 1, g, 0);  // Just 1 page

  // Setup JSON export configuration
  json_export::ExportConfig export_config;
  export_config.diagonal_weight = 0.70711;
  export_config.page_width = 4;
  export_config.page_height = 6;
  export_config.variance_weight = 0.4;
  export_config.icons_missing_weight = 0.1;
  export_config.piece_type = json_export::LAB_ICON_PIECE;

  std::string export_file = "test_export.ndjson";

  // Remove existing file if it exists
  std::remove(export_file.c_str());

  // Export the test individual
  std::cout << "Exporting test individual..." << std::endl;
  std::cout << "Fitness: " << template_individual.GetFitness() << std::endl;
  std::cout << "Generation: " << template_individual.GetBirthGeneration() << std::endl;
  std::cout << "Genome size: " << template_individual.Size() << std::endl;

  json_export::ExportIndividualToNDJSON(template_individual, export_file, export_config);

  std::cout << "\nExport successful!" << std::endl;
  std::cout << "Output file: " << export_file << std::endl;
  std::cout << "\nYou can inspect the JSON with:" << std::endl;
  std::cout << "  cat " << export_file << " | jq '.'" << std::endl;
  std::cout << "  cat " << export_file << " | jq '.pages[0].pieces[0]'" << std::endl;

  return 0;
}
