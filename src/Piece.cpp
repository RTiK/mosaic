//
// Created by Artem Khatchatourov on 14.09.20.
//

#include "Piece.h"

using namespace std;

Piece::Piece(double L, double a, double b) {
    color = color_t(L, a, b);
}

double Piece::getDistanceDelta76(Piece *other) {
    auto otherColor = other->color;
    return sqrt(
            pow(color[0]-otherColor[0], 2) +
            pow(color[1]-otherColor[1], 2) +
            pow(color[2]-otherColor[2], 2));
}