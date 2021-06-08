//
// Created by Artem Khatchatourov on 02.03.21.
//

#ifndef MOSAIC_SRC_LABPIECE_H_
#define MOSAIC_SRC_LABPIECE_H_

#include "Piece.h"


class LabPiece : public Piece {
  ColorT color_bgr_;

  // Constructs the piece from BGR values
  static ColorT LabToRgb(const ColorT &BGR);
 public:
  LabPiece(double b, double g, double r);

  ColorT GetColor() const override { return color_bgr_; };

  ColorT GetColorLab() const;
};

#endif //MOSAIC_SRC_LABPIECE_H_
