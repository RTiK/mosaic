//
// Created by Artem Khatchatourov on 14.03.21.
//

#ifndef MOSAIC_SRC_PAGE_H_
#define MOSAIC_SRC_PAGE_H_

#include "pieces/Piece.h"

/**
 * This class represents a page on the iOS device. It holds pointers to the first and the last icons of the page.
 * Iterating between these pointers traverses the genome in the individual.
 */
class Page {
 private:
  std::shared_ptr<Piece> *first_piece_, *last_piece_;
  double distances_;
  double variance_;
  double mean_color_;

  void Evaluate();

 public:
  const static unsigned int max_pieces_ = 24;

  Page(const Page &page) : first_piece_{page.first_piece_}, last_piece_{page.last_piece_},
      distances_{page.distances_}, variance_{page.variance_} {};

  Page(std::shared_ptr<Piece> *first_piece, std::shared_ptr<Piece> *last_piece);

  double GetDistances() const { return distances_; }

  double GetVariance() const { return variance_; }

  double GetMeanColor() const { return mean_color_; }

  unsigned int Size() const;

  std::shared_ptr<Piece> *GetFirstPiece() const { return first_piece_; };

  std::shared_ptr<Piece> *GetLastPiece() const { return last_piece_; };

  friend std::ostream& operator<<(std::ostream& os, Page &page);

  void Show(std::string &window_title, int side=50, ColorT default_color=ColorT(0.0, 1.0, 0.0)) const;
};

#endif //MOSAIC_SRC_PAGE_H_
