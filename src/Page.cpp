//
// Created by Artem Khatchatourov on 14.03.21.
//

#include "Page.h"
#include "PageEvaluation.h"
#include "LabPiece.h"
#ifdef _WIN32
  #include <bitset>
#endif


Page::Page(std::shared_ptr<Piece> *first_piece, std::shared_ptr<Piece> *last_piece)
    : first_piece_{first_piece}, last_piece_{last_piece} {
  // TODO check whether the array is empty
  Evaluate();
}

unsigned int Page::Size() const {
  return last_piece_ - first_piece_ + 1;
}

std::ostream &operator<<(std::ostream &os, Page &page) {
  for (std::shared_ptr<Piece>* current = page.first_piece_; current <= page.last_piece_; current++) {
    try {
      // try to cast the piece into a lab piece
      LabPiece* lab_piece = dynamic_cast<LabPiece*>(current->get());
      ColorT lab_color = lab_piece->GetColorLab();
      os << lab_color[0] << ";" << lab_color[1] << ";" << lab_color[2] << std::endl;
    } catch (std::exception exception) {
      // it must be a RGB piece then
      ColorT color = (**current).GetColor();
      os << color[0] << ";" << color[1] << ";" << color[2] << std::endl;
    }
  }
  return os;
}

void Page::Evaluate() {
  fitness_ = 0.0;
  fitness_ += page_evaluation::CalculatePageDistances(*this);
  fitness_ += page_evaluation::CalculateMissingIcons(*this) * 2;
  fitness_ += page_evaluation::CalculateColorVariance(*this) * 3;
}

