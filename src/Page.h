//
// Created by Artem Khatchatourov on 14.03.21.
//

#ifndef MOSAIC_SRC_PAGE_H_
#define MOSAIC_SRC_PAGE_H_

#include "Piece.h"

/**
 * This class represents a page on the iOS device. It holds pointers to the first and the last icons of the page.
 * Iterating between these pointers traverses the genome in the individual.
 */
enum Neighbours : unsigned char {
  N = 0x1 << 7,
  NE = 0x1 << 6,
  E = 0x1 << 5,
  SE = 0x1 << 4,
  S = 0x1 << 3,
  SW = 0x1 << 2,
  W = 0x1 << 1,
  NW = 0x1 << 0,
  ALL = N | NE | E | SE | S | SW | W | NW,
  NONE = 0x0
};

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
