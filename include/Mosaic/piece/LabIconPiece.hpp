#ifndef MOSAIC_ARRANGE_SRC_PIECES_LABICONPIECE_H_
#define MOSAIC_ARRANGE_SRC_PIECES_LABICONPIECE_H_

#include "IconPiece.hpp"


class LabIconPiece : public IconPiece {
 private:
  void Analyze(cv::Mat &color, cv::Mat &mask) override;

  cv::Mat L_histogram_, a_histogram_, b_histogram_;
  const int L_bins_ = 50;
  const int a_bins_ = 64;
  const int b_bins_ = 64;
  float L_range_[2] = {0.0f, 100.0f};
  float a_range_[2] = {-128.0f, 127.0f};
  float b_range_[2] = {-128.0f, 127.0f};

  cv::Mat lab_, mask_;
 public:
  LabIconPiece(std::string path);
  LabIconPiece(cv::Mat image);
  LabIconPiece(std::filesystem::path &path) : LabIconPiece(path.string()) {};
  LabIconPiece(const LabIconPiece &piece) : L_histogram_(piece.L_histogram_), a_histogram_(piece.b_histogram_), b_histogram_(piece.b_histogram_), IconPiece(piece.GetPath()) {};
  double Distance(const Piece &other) const override;
  static double EuclideanDistance(const LabIconPiece* p_1, const LabIconPiece* p_2);
  virtual cv::Vec3f DominatingColor() const override;
};

#endif //MOSAIC_ARRANGE_SRC_PIECES_LABICONPIECE_H_
