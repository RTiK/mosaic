//
// Created by Artem Khatchatourov on 22.04.21.
//

#ifndef MOSAIC_SRC_PAGEEVALUATION_H_
#define MOSAIC_SRC_PAGEEVALUATION_H_


#include "Piece.h"
#include "Page.h"
#include <bitset>

namespace page_evaluation {

  const unsigned int kWidth = 4;
  const unsigned int kHeight = 6;
  const unsigned int kPiecesOnPage = kWidth * kHeight;
  const float kDiagonalWeight = 0.70711;

  unsigned char CalculateNeighbors(unsigned int piece_index, unsigned int total_pieces);

  float SumUpNeighbours(unsigned char neighbours);

  float CalculatePageDistances(const Page &page);

  ColorT CalculateMeanPageColor(const Page &page);

  float CalculateColorVariance(const Page &page);

  double Evaluate(const Page &page);
};

#endif //MOSAIC_SRC_PAGEEVALUATION_H_
