//
// Created by Artem Khatchatourov on 14.09.20.
//

#ifndef MOSAIC_SRC_PIECE_H_
#define MOSAIC_SRC_PIECE_H_

#include <opencv2/opencv.hpp>

typedef cv::Vec<float, 3> ColorT;

enum Neighbours : unsigned char {
  N = 0x1 << 7,
  NE = 0x1 << 6,
  E = 0x1 << 5,
  SE = 0x1 << 4,
  S = 0x1 << 3,
  SW = 0x1 << 2,
  W = 0x1 << 1,
  NW = 0x1 << 0,
  ALL = N | NE | E | SE | S | SW | W | NW,
  NONE = 0x0
};

class Piece {
 private:
  void Init(float b, float g, float r);

 protected:
  ColorT color_;

 public:
  Piece();

  explicit Piece(float gray);

  Piece(float b, float g, float r);

  virtual double GetEuclideanDistance(const Piece &other) const;

  virtual double GetEuclideanDistance(const ColorT &other) const;

  /**
   * Getter for the color that represents the Piece internally
   * @return
   */
  virtual ColorT GetInternalColor() const { return color_; };

  /**
   * Getter for the color that can be used to display the Piece (BGR)
   * @return
   */
  virtual ColorT GetRepresentationColor() const { return color_; }

  virtual cv::Mat GetImage(int width, int height) const;
};

static std::shared_ptr<Piece> kPageBreak;


#endif //MOSAIC_SRC_PIECE_H_
