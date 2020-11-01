//
// Created by Artem Khatchatourov on 24.10.20.
//

#include <gtest/gtest.h>
#include "Individual.h"


// *...
TEST(CalculateNeighborsTests, TestUpperLeft) {
    // Piece p = Piece(0, 0, 0);
    std::vector<std::shared_ptr<Piece>> ind({std::make_shared<Piece>(0, 0, 0)});
    //std::vector<Piece> ind({&p});
    auto neighbors = Individual::calculateNeighbors(&ind.front(), &ind.front(), &ind.back());
    EXPECT_EQ(neighbors & (Neighbours::SW | Neighbours::W | Neighbours::NW | Neighbours::N | Neighbours::NE),
              Neighbours::NONE);
}

// .*..
TEST(CalculateNeighborsTests, TestUpperNextToLeft) {
    // Piece p = Piece(0, 0, 0);
    std::vector<std::shared_ptr<Piece>> ind(2, std::make_shared<Piece>(0, 0, 0));
    auto neighbors = Individual::calculateNeighbors(&ind.back(), &ind.front(), &ind.back());
    EXPECT_EQ(neighbors & (Neighbours::NW | Neighbours::N | Neighbours::NE), Neighbours::NONE);
    EXPECT_EQ(neighbors & Neighbours::W, Neighbours::W);
    EXPECT_EQ(neighbors & Neighbours::E, Neighbours::NONE);
}

// ...*
TEST(CalculateNeighborsTests, TestUpperRight) {
    // Piece p = Piece(0, 0, 0);
    std::vector<std::shared_ptr<Piece>> ind(4, std::make_shared<Piece>(0, 0, 0));
    auto neighbors = Individual::calculateNeighbors(&ind.back(), &ind.front(), &ind.back());
    EXPECT_EQ(neighbors & (Neighbours::NW | Neighbours::N | Neighbours::NE | Neighbours::E | Neighbours::SE),
              Neighbours::NONE);
    EXPECT_EQ(neighbors & Neighbours::W, Neighbours::W);
}

// ....
// *
TEST(CalculateNeighborsTests, TestMiddleLeft) {
    //Piece p = Piece(0, 0, 0);
    std::vector<std::shared_ptr<Piece>> ind(5, std::make_shared<Piece>(0, 0, 0));
    auto neighbors = Individual::calculateNeighbors(&ind.back(), &ind.front(), &ind.back());
    EXPECT_EQ(neighbors & (Neighbours::SW | Neighbours::W | Neighbours::NW), Neighbours::NONE);
    EXPECT_EQ(neighbors & (Neighbours::N | Neighbours::NE), Neighbours::N | Neighbours::NE);
}

// ....
// ...*
TEST(CalculateNeighborsTests, TestMiddleRight) {
    // Piece p = Piece(0, 0, 0);
    std::vector<std::shared_ptr<Piece>> ind(8, std::make_shared<Piece>(0, 0, 0));
    auto neighbors = Individual::calculateNeighbors(&ind.back(), &ind.front(), &ind.back());
    EXPECT_EQ(neighbors & (Neighbours::NE | Neighbours::E | Neighbours::SE), Neighbours::NONE);
    EXPECT_EQ(neighbors & (Neighbours::W | Neighbours::NW | Neighbours::N),
              Neighbours::W | Neighbours::NW | Neighbours::N);
}

// ....
// .*..
// ...
TEST(CalculateNeighborsTests, TestCenter) {
    // Piece p = Piece(0, 0, 0);
    std::vector<std::shared_ptr<Piece>> ind(13, std::make_shared<Piece>(0, 0, 0));
    auto neighbors = Individual::calculateNeighbors(&ind.at(5), &ind.front(), &ind.back());
    EXPECT_EQ(neighbors, Neighbours::ALL);
}

// ....
// .*--
// ----
TEST(CalculateNeighborsTests, TestLowerLast) {
    // Piece p = Piece(0, 0, 0);
    std::vector<std::shared_ptr<Piece>> ind(6, std::make_shared<Piece>(0, 0, 0));
    auto neighbors = Individual::calculateNeighbors(&ind.back(), &ind.front(), &ind.back());
    EXPECT_EQ(neighbors & (Neighbours::E | Neighbours::SE | Neighbours::S | Neighbours::SW), Neighbours::NONE);
    EXPECT_EQ(neighbors & Neighbours::N, Neighbours::N);
}

// ....
// .*._
// ____
TEST(CalculateNeighborsTests, TestLowerNextToLast) {
    // Piece p = Piece(0, 0, 0);
    std::vector<std::shared_ptr<Piece>> ind(7, std::make_shared<Piece>(0, 0, 0));
    auto neighbors = Individual::calculateNeighbors(&ind.at(5), &ind.front(), &ind.back());
    EXPECT_EQ(neighbors & (Neighbours::SE | Neighbours::S | Neighbours::SW), Neighbours::NONE);
}

// ....
// ..*.
// ..__
TEST(CalculateNeighborsTests, TestLowerPartyObstructed) {
    // Piece p = Piece(0, 0, 0);
    std::vector<std::shared_ptr<Piece>> ind(10, std::make_shared<Piece>(0, 0, 0));
    auto neighbor = Individual::calculateNeighbors(&ind.at(6), &ind.front(), &ind.back());
    EXPECT_EQ(neighbor & Neighbours::SW, Neighbours::SW);
    EXPECT_EQ(neighbor & (Neighbours::SE | Neighbours::S), Neighbours::NONE);
}

// ....
// .*..
// ..__
TEST(CalculateNeighborsTests, TestLowerObstructed) {
    // Piece p = Piece(0, 0, 0);
    std::vector<std::shared_ptr<Piece>> ind(10, std::make_shared<Piece>(0, 0, 0));
    auto neighbor = Individual::calculateNeighbors(&ind.at(5), &ind.front(), &ind.back());
    EXPECT_EQ(neighbor & (Neighbours::S | Neighbours::SW), Neighbours::S | Neighbours::SW);
    EXPECT_EQ(neighbor & Neighbours::SE, Neighbours::NONE);
}

// ....
// ..*.
// .___
TEST(CalculateNeighborsTests, TestLowerUnobstructed) {
    // Piece p = Piece(0, 0, 0);
    std::vector<std::shared_ptr<Piece>> ind(9, std::make_shared<Piece>(0, 0, 0));
    auto neighbor = Individual::calculateNeighbors(&ind.at(6), &ind.front(), &ind.back());
    EXPECT_EQ(neighbor & (Neighbours::SE | Neighbours::S | Neighbours::SW), Neighbours::NONE);
}

// ------
TEST(EvaluationTests, TestEvaluateSimple) {
    std::vector<std::shared_ptr<Piece>> ind(0);
    for (int i = 0; i < 10; i++) {
        Piece p = Piece(0,2, i);
        ind.emplace_back(&p);
    }
    auto i = Individual(ind);
    double a = i.evaluate();
    std::cout << a << std::endl;
}