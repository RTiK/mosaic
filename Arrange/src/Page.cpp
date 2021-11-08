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
    os << **current << std::endl;
  }
  return os;
}

void Page::Evaluate() {
  distances_ = page_evaluation::CalculatePageDistances(*this);
  //variance_ = page_evaluation::CalculateColorVariance(*this);
  // TODO add penalty for underfilled pages
}

void Page::Show(std::string &window_title, int side, cv::Vec3f default_color) const {

  cv::Mat rows[page_evaluation::kHeight];
  std::shared_ptr<Piece> *current = first_piece_;
  int type = (**current).Image(side, side).type();

  for (int i = 0; i < page_evaluation::kHeight; i++) {
    cv::Mat r[page_evaluation::kWidth];
    for (int j = 0; j < page_evaluation::kWidth; j++) {
      if (current <= last_piece_) {
        r[j] = (**current++).Image(side, side);
      } else {
        r[j] = cv::Mat(side, side, type, default_color);
      }
    }
    cv::hconcat(&r[0], page_evaluation::kWidth, rows[i]);
  }

  cv::Mat output;
  cv::vconcat(&rows[0], page_evaluation::kHeight, output);
  cv::imshow(window_title, output);
  cv::waitKey();
}
