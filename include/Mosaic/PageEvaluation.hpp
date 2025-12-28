#ifndef MOSAIC_SRC_PAGEEVALUATION_H_
#define MOSAIC_SRC_PAGEEVALUATION_H_


#include "piece/ColorPiece.hpp"
#include "Page.hpp"
#include <bitset>

namespace page_evaluation {

  const unsigned int kWidth = 4;
  const unsigned int kHeight = 6;
  const unsigned int kPiecesOnPage = kWidth * kHeight;
  const float kDiagonalWeight = 0.70711;

  unsigned char CalculateNeighbors(unsigned int piece_index, unsigned int total_pieces);

  float SumUpNeighbours(unsigned char neighbours);

  float CalculatePageDistances(const Page &page);

  std::vector<WeightedColor> CalculateColorDistribution(const Page &page);

  float CalculateVariance(const Page &page);

  int CalculateIconsMissing(const Page &page);

  double Evaluate(const Page &page);
};

#endif //MOSAIC_SRC_PAGEEVALUATION_H_
