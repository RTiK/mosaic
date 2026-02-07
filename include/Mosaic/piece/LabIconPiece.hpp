#ifndef MOSAIC_ARRANGE_SRC_LABICONPIECE_H_
#define MOSAIC_ARRANGE_SRC_LABICONPIECE_H_

#include "IconPiece.hpp"
#include <vector>

class LabIconPiece : public IconPiece {
 private:
  std::vector<WeightedColor> quantified_colors_;

  static constexpr int kClusters = 2;  // Number of color clusters

  void Analyze(cv::Mat &colors, cv::Mat &mask) override;

 public:
  explicit LabIconPiece(cv::Mat image);

  explicit LabIconPiece(std::string path);

  static double EuclideanDistance(const LabIconPiece* p_1, const LabIconPiece* p_2);

  double Distance(const Piece &other) const override;

  cv::Vec3f GetMainColor() const override;
  
  std::vector<WeightedColor> GetQuantifiedColors() const override;

  // Testing accessors
  size_t GetNumClusters() const { return kClusters; }
};

#endif //MOSAIC_ARRANGE_SRC_LABICONPIECE_H_
