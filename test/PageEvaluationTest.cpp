//
// Created by Artem Khatchatourov on 30.01.21.
//

#include <gtest/gtest.h>
#include "PageEvaluation.h"

using namespace page_evaluation;

// *...
TEST(CalculateNeighborsTests, TestSingle) {
    auto neighbors = CalculateNeighbors(0, 1);
    EXPECT_EQ(neighbors, Neighbours::NONE);
}

// o*..
TEST(CalculateNeighborsTests, TestUpperNextToLeft) {
    auto neighbors = CalculateNeighbors(1, 2);
    EXPECT_EQ(neighbors & (Neighbours::NW | Neighbours::N | Neighbours::NE), Neighbours::NONE);
    EXPECT_EQ(neighbors & Neighbours::W, Neighbours::W);
    EXPECT_EQ(neighbors & Neighbours::E, Neighbours::NONE);
}

// ooo*
// ....
TEST(CalculateNeighborsTests, TestUpperRight) {
    auto neighbors = CalculateNeighbors(3, 4);
    EXPECT_EQ(neighbors & (Neighbours::NW | Neighbours::N | Neighbours::NE | Neighbours::E | Neighbours::SE),
              Neighbours::NONE);
    EXPECT_EQ(neighbors & Neighbours::W, Neighbours::W);
}

// oooo
// *...
TEST(CalculateNeighborsTests, TestMiddleLeft) {
    auto neighbors = CalculateNeighbors(4, 5);
    EXPECT_EQ(neighbors & (Neighbours::SW | Neighbours::W | Neighbours::NW), Neighbours::NONE);
    EXPECT_EQ(neighbors & (Neighbours::N | Neighbours::NE), Neighbours::N | Neighbours::NE);
}

// oooo
// ooo*
// ....
TEST(CalculateNeighborsTests, TestMiddleRight) {
    auto neighbors = CalculateNeighbors(7, 8);
    EXPECT_EQ(neighbors & (Neighbours::NE | Neighbours::E | Neighbours::SE), Neighbours::NONE);
    EXPECT_EQ(neighbors & (Neighbours::W | Neighbours::NW | Neighbours::N),
              Neighbours::W | Neighbours::NW | Neighbours::N);
}

// oooo
// o*oo
// ooo.
TEST(CalculateNeighborsTests, TestCenter) {
    auto neighbors = CalculateNeighbors(5, 13);
    EXPECT_EQ(neighbors, Neighbours::ALL);
}

// oooo
// o*..
// ....
TEST(CalculateNeighborsTests, TestLowerLast) {
    auto neighbors = CalculateNeighbors(5, 6);
    EXPECT_EQ(neighbors & (Neighbours::E | Neighbours::SE | Neighbours::S | Neighbours::SW), Neighbours::NONE);
    EXPECT_EQ(neighbors & Neighbours::N, Neighbours::N);
}

// oooo
// o*o.
// ....
TEST(CalculateNeighborsTests, TestLowerNextToLast) {
    auto neighbors = CalculateNeighbors(5, 7);
    EXPECT_EQ(neighbors & (Neighbours::SE | Neighbours::S | Neighbours::SW), Neighbours::NONE);
}

// oooo
// oo*o
// oo..
TEST(CalculateNeighborsTests, TestLowerPartiallyObstructed) {
    auto neighbor = CalculateNeighbors(6, 10);
    EXPECT_EQ(neighbor & Neighbours::SW, Neighbours::SW);
    EXPECT_EQ(neighbor & (Neighbours::SE | Neighbours::S), Neighbours::NONE);
}

// oooo
// o*oo
// oo..
TEST(CalculateNeighborsTests, TestLowerObstructed) {
    auto neighbor = CalculateNeighbors(5, 10);
    EXPECT_EQ(neighbor & (Neighbours::S | Neighbours::SW), Neighbours::S | Neighbours::SW);
    EXPECT_EQ(neighbor & Neighbours::SE, Neighbours::NONE);
}

// oooo
// oo*o
// o...
TEST(CalculateNeighborsTests, TestLowerUnobstructed) {
    auto neighbor = CalculateNeighbors(6, 9);
    EXPECT_EQ(neighbor & (Neighbours::SE | Neighbours::S | Neighbours::SW), Neighbours::NONE);
}

TEST(EvaluationTests, TestEvaluateSimple) {
  std::vector<std::shared_ptr<Piece>> pieces {
      std::make_shared<Piece>(0.0), std::make_shared<Piece>(1.0),
      std::make_shared<Piece>(2.0), std::make_shared<Piece>(3.0),
      std::make_shared<Piece>(4.0), std::make_shared<Piece>(5.0),
      std::make_shared<Piece>(6.0), std::make_shared<Piece>(7.0),
      std::make_shared<Piece>(8.0), std::make_shared<Piece>(9.0)
  };
  Page page = Page(&pieces.front(), &pieces.back());
  double fitness = CalculatePageFitness(page, 1.0 / sqrt(2));
  EXPECT_NEAR(44.22329, fitness, 0.00001);
}

TEST(MeanPageColorTests, TestSinglePiece) {
  std::vector<std::shared_ptr<Piece>> pieces {
    std::make_shared<Piece>()
  };
  Page page = Page(&pieces.front(), &pieces.back());
  ColorT mean_color = CalculateMeanPageColor(page);
  EXPECT_EQ(ColorT(0.0, 0.0, 0.0), mean_color);
}

TEST(MeanPageColorTests, TestFullPage) {
  std::vector<std::shared_ptr<Piece>> pieces{
      std::make_shared<Piece>(0.0), std::make_shared<Piece>(1.0),
      std::make_shared<Piece>(2.0), std::make_shared<Piece>(3.0),
      std::make_shared<Piece>(4.0), std::make_shared<Piece>(5.0),
      std::make_shared<Piece>(6.0), std::make_shared<Piece>(7.0),
      std::make_shared<Piece>(8.0), std::make_shared<Piece>(9.0),
      std::make_shared<Piece>(10.0),std::make_shared<Piece>(11.0),
      std::make_shared<Piece>(12.0),std::make_shared<Piece>(13.0),
      std::make_shared<Piece>(14.0),std::make_shared<Piece>(15.0),
      std::make_shared<Piece>(16.0),std::make_shared<Piece>(17.0),
      std::make_shared<Piece>(18.0),std::make_shared<Piece>(19.0),
      std::make_shared<Piece>(20.0),std::make_shared<Piece>(21.0),
      std::make_shared<Piece>(22.0),std::make_shared<Piece>(23.0)
  };
  Page page = Page(&pieces.front(), &pieces.back());
  ColorT mean_color = CalculateMeanPageColor(page);
  EXPECT_EQ(ColorT(11.5, 11.5, 11.5), mean_color);
}

TEST(MissingPiecesTest, MissingAll) {
  std::vector<std::shared_ptr<Piece>> pieces {};
  Page page = Page(&pieces.front(), &pieces.back());
  double missing_pieces_penalty = CalculateMissingIcons(page);
  EXPECT_EQ(24, missing_pieces_penalty);
}

TEST(MissingPiecesTest, MissingAllExceptOne) {
  std::vector<std::shared_ptr<Piece>> pieces {std::make_shared<Piece>()};
  Page page = Page(&pieces.front(), &pieces.back());
  double missing_pieces_penalty = CalculateMissingIcons(page);
  EXPECT_EQ(23, missing_pieces_penalty);
}

TEST(MissingPiecesTest, MissingNone) {
  std::vector<std::shared_ptr<Piece>> pieces (24, std::make_shared<Piece>());
  Page page = Page(&pieces.front(), &pieces.back());
  double missing_pieces_penalty = page_evaluation::CalculateMissingIcons(page);
  EXPECT_EQ(0, missing_pieces_penalty);
}

TEST(ColorVarianceTest, UnicolorPage) {
  std::vector<std::shared_ptr<Piece>> pieces (24, std::make_shared<Piece>());
  Page page = Page(&pieces.front(), &pieces.back());
  double variance = CalculateColorVariance(page);
  EXPECT_EQ(0.0, variance);
}

TEST(ColorVarianceTest, SinglePiecePage) {
  std::vector<std::shared_ptr<Piece>> pieces {std::make_shared<Piece>(100.0)};
  Page page = Page(&pieces.front(), &pieces.back());
  double variance = CalculateColorVariance(page);
  EXPECT_EQ(0.0, variance);
}

TEST(ColorVarianceTest, OneVariancePage) {
  std::vector<std::shared_ptr<Piece>> pieces {
    std::make_shared<Piece>(-1.0), std::make_shared<Piece>(1.0)
  };
  Page page = Page(&pieces.front(), &pieces.back());
  double variance = CalculateColorVariance(page);
  EXPECT_NEAR(1.0, variance, 0.00001);
}
