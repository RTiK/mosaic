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
  os << "total distance: " << page.distances_ << " mean: " << page.mean_color_ << " variance: " << page.variance_ << " icons missing: " << page.icons_missing_ << std::endl;
  os << page.Size() << " icons:";
  for (std::shared_ptr<Piece>* current = page.first_piece_; current <= page.last_piece_; current++) {
    os << " " << **current;
  }
  os << std::endl;
  return os;
}

void Page::Evaluate() {
  distances_ = page_evaluation::CalculatePageDistances(*this);
  mean_color_ = page_evaluation::CalculateMeanPageColor(*this);
  variance_ = page_evaluation::CalculateVariance(*this);
  icons_missing_ = page_evaluation::CalculateIconsMissing(*this);
}

cv::Mat Page::Image(int side, cv::Vec3f default_color) const {
  cv::Mat rows[page_evaluation::kHeight + 1];
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

  cv::Mat src(1, 1, CV_32FC3, mean_color_);
  cv::Mat dst;
  cv::cvtColor(src, dst, cv::COLOR_Lab2BGR);
  cv::Vec3f bgr = dst.at<cv::Vec3f>(0, 0);
  cv::Mat color_bar (side/2, page_evaluation::kWidth * side, type, bgr);
  rows[page_evaluation::kHeight] = color_bar;

  cv::Mat image;
  cv::vconcat(&rows[0], page_evaluation::kHeight+1, image);
  return image;
}

void Page::Show(std::string &window_title, int side, cv::Vec3f default_color) const {
  cv::imshow(window_title, Image(side, default_color));
  cv::waitKey();
}
