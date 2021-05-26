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
class Page {
 private:
  std::shared_ptr<Piece> *first_piece_, *last_piece_;
  double fitness_;

  double Evaluate() const;

 public:
  const static unsigned int max_pieces_ = 24;

  Page(const Page &page) : first_piece_{page.first_piece_}, last_piece_{page.last_piece_}, fitness_{page.fitness_} {};

  Page(std::shared_ptr<Piece> *first_piece, std::shared_ptr<Piece> *last_piece);

  double GetFitness() const { return fitness_; }

  unsigned int Size() const;

  std::shared_ptr<Piece> *GetFirstPiece() const { return first_piece_; };

  std::shared_ptr<Piece> *GetLastPiece() const { return last_piece_; };
};

#endif //MOSAIC_SRC_PAGE_H_
