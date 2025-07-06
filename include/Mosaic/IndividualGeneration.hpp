#ifndef MOSAIC_SRC_INDIVIDUALGENERATION_H_
#define MOSAIC_SRC_INDIVIDUALGENERATION_H_

#include <random>
#include "Individual.hpp"

namespace individual_generation {

  Individual GenerateIndividualLabRandom(unsigned int length, unsigned int page_breaks, std::mt19937 g, int current_generation);

  Individual GenerateIndividualRgbRandom(unsigned int length, unsigned int page_breaks, std::mt19937 g, int current_generation);

  Individual GenerateIndividualGrayRandom(unsigned int length, unsigned int page_breaks, std::mt19937 g, int current_generation);

  Individual ReadRgbIcons(std::string dir_path, unsigned int page_breaks, std::mt19937 g, int current_generation);

  Individual ReadLabIcons(std::string dir_path, unsigned int page_breaks, std::mt19937 g, int current_generation);

  Individual ReadLabClusteringIcons(std::string dir_path, unsigned int page_breaks, std::mt19937 g, int current_generation);
  
}  // namespace individual_generation

#endif //MOSAIC_SRC_INDIVIDUALGENERATION_H_
