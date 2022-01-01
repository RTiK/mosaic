//
// Created by Artem Khatchatourov on 22.06.21.
//

#ifndef MOSAIC_SRC_INDIVIDUALGENERATION_H_
#define MOSAIC_SRC_INDIVIDUALGENERATION_H_

#include <random>
#include "Individual.h"

namespace individual_generation {

  Individual GenerateIndividualLabRandom(unsigned int length, unsigned int page_breaks, std::mt19937 g);

  Individual GenerateIndividualRgbRandom(unsigned int length, unsigned int page_breaks, std::mt19937 g);

  Individual GenerateIndividualGrayRandom(unsigned int length, unsigned int page_breaks, std::mt19937 g);

  Individual ReadRgbIcons(std::string dir_path, unsigned int page_breaks, std::mt19937 g);

  Individual ReadLabIcons(std::string dir_path, unsigned int page_breaks, std::mt19937 g);

  Individual ReadLabClusteringIcons(std::string dir_path, unsigned int page_breaks, std::mt19937 g);
};

#endif //MOSAIC_SRC_INDIVIDUALGENERATION_H_
