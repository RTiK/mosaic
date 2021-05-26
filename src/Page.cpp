//
// Created by Artem Khatchatourov on 14.03.21.
//

#include "Page.h"
#include "PageEvaluation.h"
#ifdef _WIN32
  #include <bitset>
#endif


Page::Page(std::shared_ptr<Piece> *first_piece, std::shared_ptr<Piece> *last_piece)
    : first_piece_{first_piece}, last_piece_{last_piece} {
  fitness_ = page_evaluation::Evaluate(*this);
}

unsigned int Page::Size() const {
  return last_piece_ - first_piece_ + 1;
}
