#ifndef MOSAIC_SRC_PIECE_H_
#define MOSAIC_SRC_PIECE_H_

#include <iostream>
#include <memory>
#include <vector>
#include <opencv2/core.hpp>

struct WeightedColor {
    cv::Vec3f color;
    float weight;
};

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

/**
 * This pure virtual class acts as an interface to different types of Piece objects.
 * Specializations of the class will have to implement the virtual methods.
 */
class Piece {
 public:

  /**
   * Returns the distance to all neighbor pieces.
   */
  virtual double Distance(const Piece &other) const = 0;

  /**
   * Returns visual representation of the piece that can be used for rendering.
   */
  virtual cv::Mat Image(int width, int height) const = 0;

  /**
   * Returns the mean color of the piece.
   */
  virtual cv::Vec3f GetMainColor() const = 0;

  /**
   * Returns a vector of weighted colors in the piece.
   */
  virtual std::vector<WeightedColor> GetQuantifiedColors() const = 0;

  /**
   * Returns a string representation of the piece.
   */
  virtual std::string Print() const = 0;

  /**
   * Overloads the << operator to print the piece.
   */
  friend std::ostream &operator<<(std::ostream &out, const Piece &piece) { return out << piece.Print(); }
};

static const std::shared_ptr<Piece> kPageBreak;

#endif //MOSAIC_SRC_PIECE_H_
