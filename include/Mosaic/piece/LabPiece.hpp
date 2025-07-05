#ifndef MOSAIC_SRC_LABPIECE_H_
#define MOSAIC_SRC_LABPIECE_H_

#include <opencv2/opencv.hpp>
#include "ColorPiece.hpp"

class LabPiece : public ColorPiece {
  cv::Vec3f lab_color_;

  // Constructs the piece from BGR values
  static cv::Vec3f LabToRgb(const cv::Vec3f &BGR);
 public:
  LabPiece(float b, float g, float r);

  virtual cv::Vec3f DominatingColor() const override { return lab_color_; }

  virtual double Distance(const Piece &other) const override;

  cv::Mat Image(int width, int height) const override;
};

#endif //MOSAIC_SRC_LABPIECE_H_
