#ifndef MOSAIC_ARRANGE_SRC_PIECES_COLORPIECE_H_
#define MOSAIC_ARRANGE_SRC_PIECES_COLORPIECE_H_

#include "Piece.hpp"

class ColorPiece : public Piece {
 private:
  void Init(float b, float g, float r);

 protected:
  cv::Vec3f original_color_;

 public:
  ColorPiece();

  explicit ColorPiece(float gray);

  ColorPiece(float b, float g, float r);

  virtual cv::Mat Image(int width, int height) const override;

  virtual cv::Vec3f DominatingColor() const override { return original_color_; };

  virtual std::vector<DominantColor> GetDominantColors() const override;

  virtual double Distance(const Piece &other) const override;

  static double EuclideanDistance(const cv::Vec3f &c_1, const cv::Vec3f &c_2);

  virtual std::string Print() const override;
};

#endif //MOSAIC_ARRANGE_SRC_PIECES_COLORPIECE_H_
