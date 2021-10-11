//
// Created by Artem Khatchatourov on 22.06.21.
//

#include <filesystem>
#include <pieces/BgrIconPiece.h>
#include "IndividualGeneration.h"
#include "pieces/LabPiece.h"

Individual individual_generation::GenerateIndividualGrayRandom(unsigned int length, unsigned int page_breaks, std::mt19937 g) {
  std::uniform_int_distribution<> random_color_value(0, 256);

  std::vector<std::shared_ptr<Piece>> genome(length + page_breaks);
  for (int i = 0; i < length; i++) {
    genome[i] = std::make_shared<Piece>(random_color_value(g) / 256.0f);
  }

  for (int i = length; i < length + page_breaks; i++) {
    genome[i] = kPageBreak;
  }
  return Individual(genome);
}

Individual individual_generation::GenerateIndividualRgbRandom(unsigned int length, unsigned int page_breaks, std::mt19937 g) {
  std::uniform_int_distribution<> random_color_value(0, 256);

  std::vector<std::shared_ptr<Piece>> genome(length + page_breaks);
  for (int i = 0; i < length; i++) {
    genome[i] = std::make_shared<Piece>(random_color_value(g) / 256.0f,
                                        random_color_value(g) / 256.0f,
                                        random_color_value(g) / 256.0f);
  }

  for (int i = length; i < length + page_breaks; i++) {
    genome[i] = kPageBreak;
  }
  return Individual(genome);
}

Individual individual_generation::GenerateIndividualLabRandom(unsigned int length, unsigned int page_breaks, std::mt19937 g) {
  std::uniform_int_distribution<> random_color_value(0, 256);

  std::vector<std::shared_ptr<Piece>> genome(length + page_breaks);
  for (int i = 0; i < length; i++) {
    genome[i] = std::make_shared<LabPiece>(random_color_value(g) / 256.0f,
                                           random_color_value(g) / 256.0f,
                                           random_color_value(g) / 256.0f);
  }

  for (int i = length; i < length + page_breaks; i++) {
    genome[i] = kPageBreak;
  }
  return Individual(genome);
}

Individual individual_generation::ReadRgbIcons(std::string dir_path, unsigned int page_breaks, std::mt19937 g) {
  assert(std::filesystem::is_directory(dir_path));  // TODO make exception
  int length = std::filesystem::hard_link_count(dir_path);
  std::cout << length << std::endl;
  std::vector<std::shared_ptr<Piece>> genome(length + page_breaks);
  for (auto file : std::filesystem::directory_iterator(dir_path)) {
    std::cout << file.path() << std::endl;
    auto piece = std::make_shared<BgrIconPiece>(file.path());
    genome.push_back(piece);
  }

  return Individual(genome);
}
