//
// Created by Artem Khatchatourov on 14.03.21.
//

#include "Page.h"
#include "PageEvaluation.h"
#ifdef _WIN32
  #include <bitset>
#endif


Page::Page(std::shared_ptr<Piece> *first_piece, std::shared_ptr<Piece> *last_piece) {
  assert(first_piece != nullptr);
  assert(last_piece != nullptr);
  assert(std::distance(first_piece, last_piece) + 1 <= page_evaluation::kPiecesOnPage);
  first_piece_ = first_piece;
  last_piece_ = last_piece;
  Evaluate();
}

unsigned int Page::Size() const {
  return last_piece_ - first_piece_ + 1;
}

std::ostream &operator<<(std::ostream &os, Page &page) {
  for (std::shared_ptr<Piece>* current = page.first_piece_; current <= page.last_piece_; current++) {
    ColorT color = (**current).GetRepresentationColor();
    os << color[0] << ";" << color[1] << ";" << color[2] << std::endl;
  }
  return os;
}

void Page::Evaluate() {
  distances_ = page_evaluation::CalculatePageDistances(*this);
  variance_ = page_evaluation::CalculateColorVariance(*this);
  // TODO add penalty for underfilled pages
}

