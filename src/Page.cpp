#include "Mosaic/Page.hpp"
#include "Mosaic/PageEvaluation.hpp"
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
  os << "total distance: " << page.distances_ << " colors: " << page.color_distribution_.size() << " variance: " << page.variance_ << " icons missing: " << page.icons_missing_ << std::endl;
  os << page.Size() << " icons:";
  for (std::shared_ptr<Piece>* current = page.first_piece_; current <= page.last_piece_; current++) {
    os << " " << **current;
  }
  os << std::endl;
  return os;
}

void Page::Evaluate() {
  distances_ = page_evaluation::CalculatePageDistances(*this);
  color_distribution_ = page_evaluation::CalculateColorDistribution(*this);
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

  // Create weighted multi-color bar
  int total_width = page_evaluation::kWidth * side;
  int bar_height = side / 2;
  std::vector<cv::Mat> segments;
  int accumulated_width = 0;

  for (size_t i = 0; i < color_distribution_.size(); i++) {
    const DominantColor& dominant_color = color_distribution_[i];

    // Calculate segment width based on weight
    int segment_width = static_cast<int>(std::round(dominant_color.weight * total_width));

    // Skip segments with 0 width (colors with tiny weights)
    if (segment_width <= 0) {
      continue;
    }

    // Create color segment
    cv::Mat src(1, 1, CV_32FC3, dominant_color.color);
    cv::Mat dst = src.clone();
    //cv::cvtColor(src, dst, cv::COLOR_Lab2BGR);  // comment this line and uncomment the previous one when operating in RGB space
    cv::Vec3f bgr = dst.at<cv::Vec3f>(0, 0);
    bgr = bgr.mul(cv::Vec3f(255, 255, 255));
    cv::Mat segment(bar_height, segment_width, type, bgr);

    segments.push_back(segment);
    accumulated_width += segment_width;
  }

  // Concatenate all segments horizontally
  cv::Mat color_bar;
  if (!segments.empty()) {
    cv::hconcat(segments, color_bar);

    // Adjust width to match exactly if rounding errors occurred
    if (color_bar.cols != total_width) {
      cv::Mat adjusted_bar;
      cv::resize(color_bar, adjusted_bar, cv::Size(total_width, bar_height), 0, 0, cv::INTER_NEAREST);
      color_bar = adjusted_bar;
    }
  } else {
    // Fallback: create empty bar if no valid segments
    color_bar = cv::Mat(bar_height, total_width, type, cv::Scalar(0, 0, 0));
  }
  rows[page_evaluation::kHeight] = color_bar;

  cv::Mat image;
  cv::vconcat(&rows[0], page_evaluation::kHeight+1, image);
  return image;
}

void Page::Show(std::string &window_title, int side, cv::Vec3f default_color) const {
  cv::imshow(window_title, Image(side, default_color));
  cv::waitKey();
}
