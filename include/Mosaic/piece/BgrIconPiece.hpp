//
// Created by Artem Khatchatourov on 17.10.21.
//

#ifndef MOSAIC_ARRANGE_SRC_HSVICONPIECE_H_
#define MOSAIC_ARRANGE_SRC_HSVICONPIECE_H_

#include "IconPiece.hpp"


class BgrIconPiece : public IconPiece {
 private:
  cv::Mat b_histogram_, g_histogram_, r_histogram_;
  const unsigned int b_bins_ = 32;
  const unsigned int g_bins_ = 32;
  const unsigned int r_bins_ = 32;

  void Analyze(cv::Mat &colors, cv::Mat &mask) override;

 public:
  // TODO decide what to do with image path
  BgrIconPiece(cv::Mat image);
  BgrIconPiece(std::string path);
  static double EuclideanDistance(const BgrIconPiece* p_1, const BgrIconPiece* p_2);
  double Distance(const Piece &other) const override;
  virtual cv::Vec3f DominatingColor() const override;
};

#endif //MOSAIC_ARRANGE_SRC_HSVICONPIECE_H_
