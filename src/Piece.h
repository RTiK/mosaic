//
// Created by Artem Khatchatourov on 14.09.20.
//

#ifndef MOSAIC_PIECE_H
#define MOSAIC_PIECE_H

#include <opencv2/opencv.hpp>

typedef cv::Vec<float, 3> ColorT;

class Piece {
 private:
  void Init(double b, double g, double r);

 protected:
  ColorT color_;
 public:
  Piece();

  explicit Piece(double gray);

  Piece(double b, double g, double r);

  virtual double GetEuclideanDistance(Piece &other);

  virtual ColorT GetColor() const { return color_; };
};

static std::shared_ptr<Piece> kPageBreak;


#endif //MOSAIC_PIECE_H
