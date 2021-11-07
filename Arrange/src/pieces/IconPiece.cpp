//
// Created by Artem Khatchatourov on 18.09.21.
//

#include "IconPiece.h"

cv::MatND IconPiece::GetHistogram(cv::Mat &colors, int channel, cv::Mat mask, int bins, float range[]) {
  cv::Mat histogram;
  int channels[] = {channel};
  int hist_size[] = {bins};
  const float* ranges[] = {range};
  cv::calcHist(&colors, 1, channels, mask, histogram, 1, hist_size, ranges);
  return histogram;
}

IconPiece::IconPiece(std::filesystem::path path) {
  icon_path_ = path;
  icon_name_ = icon_path_.filename();
  original_image_ = cv::imread(icon_path_, cv::IMREAD_UNCHANGED);
}

void IconPiece::SplitColorChannelsAndAlpha(cv::Mat image, cv::Mat &colors, cv::Mat &alpha) {
  assert(image.type() == CV_16UC4);
  colors = cv::Mat(image.rows, image.cols, CV_16UC3);
  alpha = cv::Mat(image.rows, image.cols, CV_16UC1);
  cv::Mat out[] = {colors, alpha};
  int from_to[] = {  // source channel -> channel in destination matrix
      0, 0,  // copy color channels into colors Mat
      1, 1,
      2, 2,
      3, 3   // 3rd (alpha) channel goes into the alpha Mat
  };
  cv::mixChannels(&image, 1, out, 2, from_to, 4);
  colors.convertTo(colors, CV_32F, 1.0/(USHRT_MAX+1), 0);
  alpha.convertTo(alpha, CV_8UC1);
  cv::threshold(alpha, alpha, 0.5, 1, cv::THRESH_BINARY);
}

IconPiece::IconPiece(cv::Mat image, std::string name) {
  icon_path_ = "";
  icon_name_ = name;
  original_image_ = image;
}

cv::Mat IconPiece::GetImage(int width, int height) const {
  cv::Mat resized_image;
  cv::resize(original_image_, resized_image, cv::Size(width, height));
  return resized_image;
}

std::string IconPiece::Print() const {
  return icon_name_;
}
