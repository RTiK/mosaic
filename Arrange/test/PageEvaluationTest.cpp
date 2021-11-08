//
// Created by Artem Khatchatourov on 30.01.21.
//

#include "gtest/gtest.h"
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

TEST(EvaluationTests, TestDistancesSingleLine) {
  std::vector<std::shared_ptr<Piece>> pieces {
      std::make_shared<ColorPiece>(1.0), std::make_shared<ColorPiece>(2.0),
      std::make_shared<ColorPiece>(3.0), std::make_shared<ColorPiece>(4.0)
  };
  Page page = Page(&pieces.front(), &pieces.back());
  double distances = page.GetDistances();
  EXPECT_NEAR(4.0, distances, 0.0001);
}

TEST(EvaluationTests, TestDistancesAdvanced) {
  std::vector<std::shared_ptr<Piece>> pieces {
      std::make_shared<ColorPiece>(0.0), std::make_shared<ColorPiece>(1.0),
      std::make_shared<ColorPiece>(2.0), std::make_shared<ColorPiece>(3.0),
      std::make_shared<ColorPiece>(4.0), std::make_shared<ColorPiece>(5.0),
      std::make_shared<ColorPiece>(6.0), std::make_shared<ColorPiece>(7.0),
      std::make_shared<ColorPiece>(8.0), std::make_shared<ColorPiece>(9.0)
  };
  Page page = Page(&pieces.front(), &pieces.back());
  double distances = page.GetDistances();
  EXPECT_NEAR(106.8456, distances, 0.0001);
}

TEST(EvaluationTests, TestVarianceSimple) {
  std::vector<std::shared_ptr<Piece>> pieces {
      std::make_shared<ColorPiece>(0.0), std::make_shared<ColorPiece>(1.0),
      std::make_shared<ColorPiece>(2.0), std::make_shared<ColorPiece>(3.0),
      std::make_shared<ColorPiece>(4.0), std::make_shared<ColorPiece>(5.0),
      std::make_shared<ColorPiece>(6.0), std::make_shared<ColorPiece>(7.0),
      std::make_shared<ColorPiece>(8.0), std::make_shared<ColorPiece>(9.0)
  };
  Page page = Page(&pieces.front(), &pieces.back());
  double variance = page.GetVariance();
  EXPECT_NEAR(82.5, variance, 0.0001);
}

TEST(EvaluationTests, TestVarianceSinglePiece) {
  std::vector<std::shared_ptr<Piece>> pieces {std::make_shared<ColorPiece>(0.5)};
  Page page = Page(&pieces.front(), &pieces.back());
  double variance = page.GetVariance();
  EXPECT_NEAR(0.0, variance, 0.0001);
}
/*
TEST(MeanPageColorTests, TestSinglePiece) {
  std::vector<std::shared_ptr<Piece>> pieces {
    std::make_shared<ColorPiece>()
  };
  Page page = Page(&pieces.front(), &pieces.back());
  ColorT mean_color = CalculateMeanPageColor(page);
  EXPECT_EQ(ColorT(0.0, 0.0, 0.0), mean_color);
}

TEST(MeanPageColorTests, TestFullPage) {
  std::vector<std::shared_ptr<Piece>> pieces{
      std::make_shared<ColorPiece>(0.0), std::make_shared<ColorPiece>(1.0),
      std::make_shared<ColorPiece>(2.0), std::make_shared<ColorPiece>(3.0),
      std::make_shared<ColorPiece>(4.0), std::make_shared<ColorPiece>(5.0),
      std::make_shared<ColorPiece>(6.0), std::make_shared<ColorPiece>(7.0),
      std::make_shared<ColorPiece>(8.0), std::make_shared<ColorPiece>(9.0),
      std::make_shared<ColorPiece>(10.0),std::make_shared<ColorPiece>(11.0),
      std::make_shared<ColorPiece>(12.0),std::make_shared<ColorPiece>(13.0),
      std::make_shared<ColorPiece>(14.0),std::make_shared<ColorPiece>(15.0),
      std::make_shared<ColorPiece>(16.0),std::make_shared<ColorPiece>(17.0),
      std::make_shared<ColorPiece>(18.0),std::make_shared<ColorPiece>(19.0),
      std::make_shared<ColorPiece>(20.0),std::make_shared<ColorPiece>(21.0),
      std::make_shared<ColorPiece>(22.0),std::make_shared<ColorPiece>(23.0)
  };
  Page page = Page(&pieces.front(), &pieces.back());
  ColorT mean_color = CalculateMeanPageColor(page);
  EXPECT_EQ(ColorT(11.5, 11.5, 11.5), mean_color);
}
*/
TEST(PageSizeTest, Zero) {
  std::vector<std::shared_ptr<Piece>> pieces {};
  EXPECT_DEATH(Page(&pieces.front(), &pieces.back()), "Assertion failed*");
}

TEST(PageSizeTest, One) {
  std::vector<std::shared_ptr<Piece>> pieces {std::make_shared<ColorPiece>()};
  Page page = Page(&pieces.front(), &pieces.back());
  unsigned int size = page.Size();
  EXPECT_EQ(1, size);
}

TEST(PageSizeTest, All) {
  std::vector<std::shared_ptr<Piece>> pieces (kPiecesOnPage - 1, std::make_shared<ColorPiece>());
  Page page = Page(&pieces.front(), &pieces.back());
  unsigned int size = page.Size();
  EXPECT_EQ(kPiecesOnPage - 1, size);
}

TEST(PageSizeTest, AllExceptOne) {
  std::vector<std::shared_ptr<Piece>> pieces (kPiecesOnPage, std::make_shared<ColorPiece>());
  Page page = Page(&pieces.front(), &pieces.back());
  unsigned int size = page.Size();
  EXPECT_EQ(kPiecesOnPage, size);
}

TEST(ColorVarianceTest, UnicolorPage) {
  std::vector<std::shared_ptr<Piece>> pieces (kPiecesOnPage, std::make_shared<ColorPiece>());
  Page page = Page(&pieces.front(), &pieces.back());
  double variance = page.GetVariance();
  EXPECT_EQ(0.0, variance);
}

TEST(ColorVarianceTest, SinglePiecePage) {
  std::vector<std::shared_ptr<Piece>> pieces {std::make_shared<ColorPiece>(100.0)};
  Page page = Page(&pieces.front(), &pieces.back());
  double variance = page.GetVariance();
  EXPECT_EQ(0.0, variance);
}

TEST(ColorVarianceTest, OneVariancePage) {
  std::vector<std::shared_ptr<Piece>> pieces {
    std::make_shared<ColorPiece>(-1.0), std::make_shared<ColorPiece>(1.0)
  };
  Page page = Page(&pieces.front(), &pieces.back());
  double variance = page.GetVariance();
  EXPECT_NEAR(2.0, variance, 0.0001);
}

TEST(SumUpNeighborsTest, Empty) {
  float sum = SumUpNeighbours(Neighbours::NONE);
  EXPECT_EQ(sum, 0.0f);
}

TEST(SumUpNeighborsTest, Full) {
  float sum = SumUpNeighbours(Neighbours::ALL);
  EXPECT_EQ(sum, 4.0f + 4.0f * kDiagonalWeight);
}

TEST(SumUpNeighborsTest, Diagonal) {
  unsigned char neighbours = Neighbours::NE | Neighbours::SE | Neighbours::SW | Neighbours::NW;
  float sum = SumUpNeighbours(neighbours);
  EXPECT_EQ(sum, 4.0f * kDiagonalWeight);
}

TEST(SumUpNeighborsTest, Normal) {
  unsigned char neighbours = Neighbours::N | Neighbours::E | Neighbours::S | Neighbours::W;
  float sum = SumUpNeighbours(neighbours);
  EXPECT_EQ(sum, 4.0f);
}