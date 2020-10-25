//
// Created by Artem Khatchatourov on 14.09.20.
//

#ifndef MOSAIC_PIECE_H
#define MOSAIC_PIECE_H

#include <opencv2/opencv.hpp>

typedef cv::Vec<double, 3> color_t;


class Piece {
    color_t color;

public:
    Piece(double L, double a, double b);

    double getDistanceDelta76(Piece* other);
};

#endif //MOSAIC_PIECE_H
