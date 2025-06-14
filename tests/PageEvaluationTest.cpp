#include <gtest/gtest.h>
#include <Mosaic/PageEvaluation.hpp>

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
  EXPECT_NEAR(6.9282, distances, 0.0001);
}

TEST(EvaluationTests, TestDistances) {
  std::vector<std::shared_ptr<Piece>> pieces {
      std::make_shared<ColorPiece>(0.0), std::make_shared<ColorPiece>(1.0),
      std::make_shared<ColorPiece>(2.0), std::make_shared<ColorPiece>(3.0),
      std::make_shared<ColorPiece>(4.0), std::make_shared<ColorPiece>(5.0),
      std::make_shared<ColorPiece>(6.0), std::make_shared<ColorPiece>(7.0),
      std::make_shared<ColorPiece>(8.0), std::make_shared<ColorPiece>(9.0)
  };
  Page page = Page(&pieces.front(), &pieces.back());
  double distances = page.GetDistances();
  EXPECT_NEAR(50.0304, distances, 0.0001);
}

TEST(EvaluationTests, TestVariance) {
  std::vector<std::shared_ptr<Piece>> pieces {
      std::make_shared<ColorPiece>(0.0), std::make_shared<ColorPiece>(1.0),
      std::make_shared<ColorPiece>(2.0), std::make_shared<ColorPiece>(3.0),
      std::make_shared<ColorPiece>(4.0), std::make_shared<ColorPiece>(5.0),
      std::make_shared<ColorPiece>(6.0), std::make_shared<ColorPiece>(7.0),
      std::make_shared<ColorPiece>(8.0), std::make_shared<ColorPiece>(9.0)
  };
  Page page = Page(&pieces.front(), &pieces.back());
  double variance = page.GetVariance();
  EXPECT_NEAR(43.3012, variance, 0.0001);
}

TEST(EvaluationTests, TestVarianceSinglePiece) {
  std::vector<std::shared_ptr<Piece>> pieces {std::make_shared<ColorPiece>(0.5)};
  Page page = Page(&pieces.front(), &pieces.back());
  double variance = page.GetVariance();
  EXPECT_NEAR(0.0, variance, 0.0001);
}

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
  EXPECT_NEAR(3.4641, variance, 0.0001);
}

TEST(MissingIconsTest, NoIconsMissing) {
  std::vector<std::shared_ptr<Piece>> pieces {kPiecesOnPage, std::make_shared<ColorPiece>(0.0)};
  Page page = Page(&pieces.front(), &pieces.back());
  double icons_missing = page.GetIconsMissing();
  EXPECT_EQ(0, icons_missing);
}

TEST(MissingIconsTest, OneIconMissing) {
  int exp_icons_missing = 1;
  std::vector<std::shared_ptr<Piece>> pieces {kPiecesOnPage-exp_icons_missing, std::make_shared<ColorPiece>(0.0)};
  Page page = Page(&pieces.front(), &pieces.back());
  double icons_missing = page.GetIconsMissing();
  EXPECT_EQ(exp_icons_missing, icons_missing);
}

TEST(MissingIconsTest, AllButOneIconsMissing) {
  std::vector<std::shared_ptr<Piece>> pieces {std::make_shared<ColorPiece>(0.0)};
  Page page = Page(&pieces.front(), &pieces.back());
  double icons_missing = page.GetIconsMissing();
  EXPECT_EQ(kPiecesOnPage-1, icons_missing);
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