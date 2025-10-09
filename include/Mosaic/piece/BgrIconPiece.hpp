#ifndef MOSAIC_ARRANGE_SRC_HSVICONPIECE_H_
#define MOSAIC_ARRANGE_SRC_HSVICONPIECE_H_

#include "IconPiece.hpp"
#include <vector>

struct DominantColor {
    cv::Vec3f color;
    float weight;
};

class BgrIconPiece : public IconPiece {
 private:
    std::vector<DominantColor> dominant_colors_;
    static constexpr int kClusters = 2;  // Number of color clusters

    void Analyze(cv::Mat &colors, cv::Mat &mask) override;

 public:
  // TODO(artem): Handle image path management
  explicit BgrIconPiece(cv::Mat image);
  explicit BgrIconPiece(std::string path);
  static double EuclideanDistance(const BgrIconPiece* p_1, const BgrIconPiece* p_2);
  double Distance(const Piece &other) const override;
  virtual cv::Vec3f DominatingColor() const override;
};

#endif //MOSAIC_ARRANGE_SRC_HSVICONPIECE_H_
