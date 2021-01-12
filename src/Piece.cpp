//
// Created by Artem Khatchatourov on 14.09.20.
//

#include "Piece.h"

Piece::Piece(double gray) {
    Piece(gray, gray, gray);
}

Piece::Piece(double r, double g, double b) {
    color = color_t(r, g, b);
}

double Piece::getDistanceDelta76(Piece* other) {
    auto otherColor = other->color;
    return sqrt(
            pow(color[0]-otherColor[0], 2) +
            pow(color[1]-otherColor[1], 2) +
            pow(color[2]-otherColor[2], 2));
}

double Piece::getNormalizedDistance() {
    return cummultativeDistance / std::bitset<8>(neighbours).count();
}

void Piece::setNeighbors(unsigned int neighbours) {
    this->neighbours = neighbours;
}

unsigned int Piece::getNeighbors() {
    return this->neighbours;
}

void Piece::setCummultativeDistance(double cummultativeDistance) {
    this->cummultativeDistance = cummultativeDistance;
}

double Piece::getCummultativeDistance() {
    return this->cummultativeDistance;
}
