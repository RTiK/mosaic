//
// Created by Artem Khatchatourov on 22.06.21.
//

#include "IndividualGeneration.h"
#include "LabPiece.h"

Individual individual_generation::GenerateIndividualGrayRandom(unsigned int length, unsigned int page_breaks, std::mt19937 g) {
  std::uniform_int_distribution<> random_color_value(0, 256);

  std::vector<std::shared_ptr<Piece>> genome(length + page_breaks);
  for (int i = 0; i < length; i++) {
    genome[i] = std::make_shared<Piece>(random_color_value(g) / 256.0);
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
    genome[i] = std::make_shared<Piece>(random_color_value(g) / 256.0,
                                        random_color_value(g) / 256.0,
                                        random_color_value(g) / 256.0);
  }

  for (int i = length; i < length + page_breaks; i++) {
    genome[i] = kPageBreak;
  }
  return Individual(genome);
}

Individual individual_generation::GenerateIndividualLabRandom(unsigned int length,
                                                              unsigned int page_breaks,
                                                              std::mt19937 g) {
  std::uniform_int_distribution<> random_color_value(0, 256);

  std::vector<std::shared_ptr<Piece>> genome(length + page_breaks);
  for (int i = 0; i < length; i++) {
    genome[i] = std::make_shared<LabPiece>(random_color_value(g) / 256.0,
                                           random_color_value(g) / 256.0,
                                           random_color_value(g) / 256.0);
  }

  for (int i = length; i < length + page_breaks; i++) {
    genome[i] = kPageBreak;
  }
  return Individual(genome);
}
