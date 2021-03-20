//
// Created by Artem Khatchatourov on 14.03.21.
//

#ifndef MOSAIC_SRC_PAGE_H_
#define MOSAIC_SRC_PAGE_H_

#include "Piece.h"

/**
 * This class represents a page on the iOS device. It holds the pointers to the first and the last icons of the page.
 * Iterating between these pointers traverses the genome on the individual.
 */
class Page {
 private:
  std::shared_ptr<Piece> *first_piece_, *last_piece_;
  const unsigned int page_width_ = 4, page_height_ = 6;
  double fitness_;
  const double diagonal_weight_ = 1 / sqrt(2);

  double Evaluate(std::shared_ptr<Piece> *first_piece, std::shared_ptr<Piece> *last_piece);
 public:
  const static unsigned int max_pieces_ = 24;

  Page(const Page &page) : first_piece_{page.first_piece_}, last_piece_{page.last_piece_}, fitness_{page.fitness_} {};

  Page(std::shared_ptr<Piece> *first_piece, std::shared_ptr<Piece> *last_piece);

  double GetFitness() const { return fitness_; }

  std::shared_ptr<Piece> *GetFirstPiece() const { return first_piece_; };

  std::shared_ptr<Piece> *GetLastPiece() const { return last_piece_; };

  static unsigned int CalculateNeighbors(std::shared_ptr<Piece> *current,
                                         std::shared_ptr<Piece> *first,
                                         std::shared_ptr<Piece> *last);
};

#endif //MOSAIC_SRC_PAGE_H_
