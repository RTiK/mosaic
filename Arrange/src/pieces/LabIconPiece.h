//
// Created by Artem Khatchatourov on 13.11.21.
//

#ifndef MOSAIC_ARRANGE_SRC_PIECES_LABICONPIECE_H_
#define MOSAIC_ARRANGE_SRC_PIECES_LABICONPIECE_H_

#include "IconPiece.h"


class LabIconPiece : public IconPiece {
 private:
  void Analyze(cv::Mat &color, cv::Mat &mask) override;

  cv::Mat L_histogram_, a_histogram_, b_histogram_;

  cv::Mat lab_, mask_;
 public:
  LabIconPiece(std::string path);
  LabIconPiece(cv::Mat image);
  LabIconPiece(std::filesystem::path &path) : LabIconPiece(path.string()) {};
  LabIconPiece(const LabIconPiece &piece) : L_histogram_(piece.L_histogram_), a_histogram_(piece.b_histogram_), b_histogram_(piece.b_histogram_), IconPiece(piece.GetPath()) {};
  double Distance(const Piece &other) const override;
  friend double EuclideanDistance(const LabIconPiece* p_1, const LabIconPiece* p_2);
  virtual cv::Vec3f DominatingColor() const override;
};

#endif //MOSAIC_ARRANGE_SRC_PIECES_LABICONPIECE_H_
