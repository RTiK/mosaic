#ifndef MOSAIC_SRC_INDIVIDUALGENERATION_H_
#define MOSAIC_SRC_INDIVIDUALGENERATION_H_

#include <random>
#include "Individual.hpp"

namespace individual_generation {

  /**
   * Generates an individual with random Lab colors.
   */
  Individual GenerateIndividualLabRandom(unsigned int length, unsigned int page_breaks, std::mt19937 g, int current_generation);

  /**
   * Generates an individual with random RGB colors.
   */
  Individual GenerateIndividualRgbRandom(unsigned int length, unsigned int page_breaks, std::mt19937 g, int current_generation);

  /**
   * Generates an individual with random grayscale colors. To this end, we use normal ColorPieces where all three color channels have the same value.
   */
  Individual GenerateIndividualGrayRandom(unsigned int length, unsigned int page_breaks, std::mt19937 g, int current_generation);

  /**
   * Reads all icon images from the specified directory and creates LabIconPieces from them.
   */
  Individual ReadRgbIcons(std::string dir_path, unsigned int page_breaks, std::mt19937 g, int current_generation);

}  // namespace individual_generation

#endif //MOSAIC_SRC_INDIVIDUALGENERATION_H_
