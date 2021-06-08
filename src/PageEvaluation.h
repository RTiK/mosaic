//
// Created by Artem Khatchatourov on 22.04.21.
//

#ifndef MOSAIC_SRC_PAGEEVALUATION_H_
#define MOSAIC_SRC_PAGEEVALUATION_H_


#include "Piece.h"
#include "Page.h"
#include <bitset>

namespace page_evaluation {

  unsigned int CalculateNeighbors(unsigned int piece_index, unsigned int total_pieces);

  double CalculatePageDistances(const Page &page, double diagonal_weight= 0.70711);

  ColorT CalculateMeanPageColor(const Page &page);

  double CalculateColorVariance(const Page &page);

  double CalculateMissingIcons(const Page &page);

  double Evaluate(const Page &page);
};

#endif //MOSAIC_SRC_PAGEEVALUATION_H_
