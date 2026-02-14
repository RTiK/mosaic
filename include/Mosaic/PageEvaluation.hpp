#ifndef MOSAIC_SRC_PAGEEVALUATION_H_
#define MOSAIC_SRC_PAGEEVALUATION_H_

#include "Page.hpp"

/**
 * Functions related to evaluating the fitness of a page.
 */
namespace page_evaluation {

  // Number of pieces in a row on a page
  const unsigned int kHeight = 6;
  // Number of pieces in a column on a page
  const unsigned int kWidth = 4;
  // Total maximum number of pieces on a page
  const unsigned int kPiecesOnPage = kWidth * kHeight;
  // Weight to apply to diagonal neighbors
  const float kDiagonalWeight = 0.70711;

  const double kVarianceWeight = 1.8;
  const double kMissingIconsWeight = 0.5;

  /**
   * Calculates which neighbors a piece around it.
   * @return A bitmask of neighbors composed of Neighbors enum values.
   */
  unsigned char CalculateNeighbors(unsigned int piece_index, unsigned int total_pieces);

  /**
   * Sums up neighbor count from the bitmask while applying diagonal weight to diagonal neighbors.
   */
  float SumUpNeighbours(unsigned char neighbours);

  /**
   * Calculates the total distance of every piece on the page using the piece's own distance implementation.
   */
  float CalculatePageDistances(const Page &page);

  /**
   * Returns all weighted colors of all icons on the page sorted by weight in descending order.
   */
  std::vector<WeightedColor> CalculateColorDistribution(const Page &page);

  /**
   * Calculates color variance of the page using euclidean distance. Color variance is the total of distances of all 
   * icons on the page from the mean color of the page.
   */
  float CalculateVariance(const Page &page);

  /**
   * Calculates the number of missing icons on a page.
   */
  int CalculateIconsMissing(const Page &page);

}; // namespace page_evaluation

#endif //MOSAIC_SRC_PAGEEVALUATION_H_
