#ifndef MOSAIC_ARRANGE_SRC_PIECES_LABICONCLUSTERINGPIECE_H_
#define MOSAIC_ARRANGE_SRC_PIECES_LABICONCLUSTERINGPIECE_H_

#include <opencv2/opencv.hpp>
#include "IconPiece.hpp"

class LabIconClusteringPiece : public IconPiece {

  std::vector<cv::Vec3f> centers_;
  std::vector<int> weights_;
  cv::Vec3f dominating_color_;

  void Analyze(cv::Mat &color, cv::Mat &mask) override;
  std::vector<int> ClusterSizes(cv::Mat labels);
  void DetermineDominatingColor();
  void ShowClusters(cv::Mat icon_array, cv::Mat mask);
 public:
  LabIconClusteringPiece(std::string path);
  LabIconClusteringPiece(cv::Mat image);

  double Distance(const Piece &other) const override;
  static double EuclideanDistance(const LabIconClusteringPiece* p_1, const LabIconClusteringPiece* p_2);
  cv::Vec3f DominatingColor() const override { return dominating_color_; };
  std::vector<DominantColor> GetDominantColors() const override;

};

#endif //MOSAIC_ARRANGE_SRC_PIECES_LABICONCLUSTERINGPIECE_H_
