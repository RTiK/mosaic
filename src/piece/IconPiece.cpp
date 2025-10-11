#include "Mosaic/piece/IconPiece.hpp"

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

IconPiece::IconPiece(cv::Mat image, std::string name) {
  icon_path_ = "";
  icon_name_ = name;
  original_image_ = image;
}

void IconPiece::SplitColorChannelsAndAlpha(const cv::Mat &image, cv::Mat &colors, cv::Mat &alpha) {
  assert(image.type() == CV_8UC4 || image.type() == CV_16UC4);

  // Determine the appropriate intermediate types based on input
  int intermediate_color_type = (image.type() == CV_16UC4) ? CV_16UC3 : CV_8UC3;
  int intermediate_alpha_type = (image.type() == CV_16UC4) ? CV_16UC1 : CV_8UC1;

  colors = cv::Mat(image.rows, image.cols, intermediate_color_type);
  alpha = cv::Mat(image.rows, image.cols, intermediate_alpha_type);
  cv::Mat out[] = {colors, alpha};
  int from_to[] = {  // source channel -> channel in destination matrix
      0, 0,  // copy color channels into colors Mat
      1, 1,
      2, 2,
      3, 3   // 3rd (alpha) channel goes into the alpha Mat
  };

  cv::mixChannels(&image, 1, out, 2, from_to, 4);

  // Normalize based on input type
  if (image.type() == CV_16UC4) {
    colors.convertTo(colors, CV_32F, 1.0/(USHRT_MAX+1), 0);
    alpha.convertTo(alpha, CV_8UC1, 1.0/(USHRT_MAX+1), 0);
  } else {
    colors.convertTo(colors, CV_32F, 1.0/256.0, 0);
    alpha.convertTo(alpha, CV_8UC1);
  }

  cv::threshold(alpha, alpha, 0.5, 1, cv::THRESH_BINARY);
}

cv::Mat IconPiece::Image(int width, int height) const {
  cv::Mat resized_image, converted_image;
  cv::resize(original_image_, resized_image, cv::Size(width, height));
  //resized_image.convertTo(converted_image, CV_32F, 1.0/(USHRT_MAX+1), 0);
  return resized_image;
}

std::string IconPiece::Print() const {
  return icon_name_;
}
