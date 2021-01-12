//
// Created by Artem Khatchatourov on 14.09.20.
//

#ifndef MOSAIC_PIECE_H
#define MOSAIC_PIECE_H

#include <opencv2/opencv.hpp>

typedef cv::Vec<double, 3> color_t;

enum Neighbours: unsigned int {
    N   = 0x1 << 7,
    NE  = 0x1 << 6,
    E   = 0x1 << 5,
    SE  = 0x1 << 4,
    S   = 0x1 << 3,
    SW  = 0x1 << 2,
    W   = 0x1 << 1,
    NW  = 0x1 << 0,
    ALL = N | NE | E | SE | S | SW | W | NW,
    NONE= 0x0
};


class Piece {
    color_t color;

    double cummultativeDistance;

    unsigned int neighbours;

    void init(double r, double g, double b);

public:
    Piece();

    explicit Piece(double gray);

    Piece(double r, double g, double b);

    double getDistanceDelta76(Piece* other);

    void setNeighbors(unsigned int neighbours);

    unsigned int getNeighbors();

    void setCummultativeDistance(double cummultativeDistance);

    double getCummultativeDistance();

    double getNormalizedDistance();
};

#endif //MOSAIC_PIECE_H
