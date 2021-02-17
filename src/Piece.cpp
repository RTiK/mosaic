//
// Created by Artem Khatchatourov on 14.09.20.
//

#include "Piece.h"

Piece::Piece() {
    init(0.0, 0.0, 0.0);
}

Piece::Piece(double gray) {
    init(gray, gray, gray);
}

Piece::Piece(double r, double g, double b) {
    init(r, g, b);
}

void Piece::init(double r, double g, double b) {
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
    unsigned int num_of_neighbors = neighbours > 0 ? std::bitset<8>(neighbours).count() : 1;
    return cummultativeDistance / num_of_neighbors;
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

color_t Piece::getColor() {
    return color_t(color);
}

