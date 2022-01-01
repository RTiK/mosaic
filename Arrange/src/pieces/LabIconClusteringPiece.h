//
// Created by Artem Khatchatourov on 30.12.21.
//

#ifndef MOSAIC_ARRANGE_SRC_PIECES_LABICONCLUSTERINGPIECE_H_
#define MOSAIC_ARRANGE_SRC_PIECES_LABICONCLUSTERINGPIECE_H_

#include <opencv2/opencv.hpp>
#include "IconPiece.h"

class LabIconClusteringPiece : public IconPiece {

  std::vector<cv::Vec3f> centers_;
  std::vector<double> weights_;

  void Analyze(cv::Mat &color, cv::Mat &mask) override;
  void DetermineWeights(cv::Mat &labels);
 public:
  LabIconClusteringPiece(std::string path);
  LabIconClusteringPiece(cv::Mat image);

  double Distance(const Piece &other) const override;
  friend double EuclideanDistance(const LabIconClusteringPiece* p_1, const LabIconClusteringPiece* p_2);
  cv::Vec3f DominatingColor() const override;

};

#endif //MOSAIC_ARRANGE_SRC_PIECES_LABICONCLUSTERINGPIECE_H_
