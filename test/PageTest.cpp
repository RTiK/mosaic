//
// Created by Artem Khatchatourov on 30.01.21.
//

#include <gtest/gtest.h>
#include "Page.h"
#include "Individual.h"


TEST(CalculateNeighborsTests, TestUpperLeft) {
    std::vector<std::shared_ptr<Piece>> ind({std::make_shared<Piece>()});
    auto neighbors = Page::CalculateNeighbors(&ind.front(), &ind.front(), &ind.back());
    EXPECT_EQ(neighbors & (Neighbours::SW | Neighbours::W | Neighbours::NW | Neighbours::N | Neighbours::NE),
              Neighbours::NONE);
}

// o*oo
TEST(CalculateNeighborsTests, TestUpperNextToLeft) {
    std::vector<std::shared_ptr<Piece>> ind(2, std::make_shared<Piece>());
    auto neighbors = Page::CalculateNeighbors(&ind.back(), &ind.front(), &ind.back());
    EXPECT_EQ(neighbors & (Neighbours::NW | Neighbours::N | Neighbours::NE), Neighbours::NONE);
    EXPECT_EQ(neighbors & Neighbours::W, Neighbours::W);
    EXPECT_EQ(neighbors & Neighbours::E, Neighbours::NONE);
}

// ooo*
TEST(CalculateNeighborsTests, TestUpperRight) {
    std::vector<std::shared_ptr<Piece>> ind(4, std::make_shared<Piece>());
    auto neighbors = Page::CalculateNeighbors(&ind.back(), &ind.front(), &ind.back());
    EXPECT_EQ(neighbors & (Neighbours::NW | Neighbours::N | Neighbours::NE | Neighbours::E | Neighbours::SE),
              Neighbours::NONE);
    EXPECT_EQ(neighbors & Neighbours::W, Neighbours::W);
}

// oooo
// *
TEST(CalculateNeighborsTests, TestMiddleLeft) {
    std::vector<std::shared_ptr<Piece>> ind(5, std::make_shared<Piece>());
    auto neighbors = Page::CalculateNeighbors(&ind.back(), &ind.front(), &ind.back());
    EXPECT_EQ(neighbors & (Neighbours::SW | Neighbours::W | Neighbours::NW), Neighbours::NONE);
    EXPECT_EQ(neighbors & (Neighbours::N | Neighbours::NE), Neighbours::N | Neighbours::NE);
}

// oooo
// ooo*
TEST(CalculateNeighborsTests, TestMiddleRight) {
    std::vector<std::shared_ptr<Piece>> ind(8, std::make_shared<Piece>());
    auto neighbors = Page::CalculateNeighbors(&ind.back(), &ind.front(), &ind.back());
    EXPECT_EQ(neighbors & (Neighbours::NE | Neighbours::E | Neighbours::SE), Neighbours::NONE);
    EXPECT_EQ(neighbors & (Neighbours::W | Neighbours::NW | Neighbours::N),
              Neighbours::W | Neighbours::NW | Neighbours::N);
}

// oooo
// o*oo
// ooo
TEST(CalculateNeighborsTests, TestCenter) {
    std::vector<std::shared_ptr<Piece>> ind(13, std::make_shared<Piece>());
    auto neighbors = Page::CalculateNeighbors(&ind.at(5), &ind.front(), &ind.back());
    EXPECT_EQ(neighbors, Neighbours::ALL);
}

// oooo
// o*..
// ....
TEST(CalculateNeighborsTests, TestLowerLast) {
    std::vector<std::shared_ptr<Piece>> ind(6, std::make_shared<Piece>());
    auto neighbors = Page::CalculateNeighbors(&ind.back(), &ind.front(), &ind.back());
    EXPECT_EQ(neighbors & (Neighbours::E | Neighbours::SE | Neighbours::S | Neighbours::SW), Neighbours::NONE);
    EXPECT_EQ(neighbors & Neighbours::N, Neighbours::N);
}

// oooo
// o*o.
// ....
TEST(CalculateNeighborsTests, TestLowerNextToLast) {
    std::vector<std::shared_ptr<Piece>> ind(7, std::make_shared<Piece>());
    auto neighbors = Page::CalculateNeighbors(&ind.at(5), &ind.front(), &ind.back());
    EXPECT_EQ(neighbors & (Neighbours::SE | Neighbours::S | Neighbours::SW), Neighbours::NONE);
}

// oooo
// oo*o
// oo..
TEST(CalculateNeighborsTests, TestLowerPartyObstructed) {
    std::vector<std::shared_ptr<Piece>> ind(10, std::make_shared<Piece>());
    auto neighbor = Page::CalculateNeighbors(&ind.at(6), &ind.front(), &ind.back());
    EXPECT_EQ(neighbor & Neighbours::SW, Neighbours::SW);
    EXPECT_EQ(neighbor & (Neighbours::SE | Neighbours::S), Neighbours::NONE);
}

// oooo
// o*oo
// oo..
TEST(CalculateNeighborsTests, TestLowerObstructed) {
    std::vector<std::shared_ptr<Piece>> ind(10, std::make_shared<Piece>());
    auto neighbor = Page::CalculateNeighbors(&ind.at(5), &ind.front(), &ind.back());
    EXPECT_EQ(neighbor & (Neighbours::S | Neighbours::SW), Neighbours::S | Neighbours::SW);
    EXPECT_EQ(neighbor & Neighbours::SE, Neighbours::NONE);
}

// oooo
// oo*o
// o...
TEST(CalculateNeighborsTests, TestLowerUnobstructed) {
    std::vector<std::shared_ptr<Piece>> ind(9, std::make_shared<Piece>());
    auto neighbor = Page::CalculateNeighbors(&ind.at(6), &ind.front(), &ind.back());
    EXPECT_EQ(neighbor & (Neighbours::SE | Neighbours::S | Neighbours::SW), Neighbours::NONE);
}

// oooo  2...
// o1..
TEST(CalculateNeighborsTest, TestPageBreak) {
    std::vector<std::shared_ptr<Piece>> ind(6, std::make_shared<Piece>());
    ind.push_back(kPageBreak);
    ind.push_back(std::shared_ptr<Piece>());
    auto neighbor_1 = Page::CalculateNeighbors(&ind.at(5), &ind.front(), &ind.at(5));
    auto neighbor_2 = Page::CalculateNeighbors(&ind.at(6), &ind.at(6), &ind.at(6));
    EXPECT_EQ(neighbor_1 & (Neighbours::E | Neighbours::SE | Neighbours::S | Neighbours::SW),Neighbours::NONE);
    EXPECT_EQ(neighbor_2, Neighbours::NONE);
}

TEST(EvaluationTests, TestEvaluateSimple) {
  std::vector<std::shared_ptr<Piece>> ind{
      std::make_shared<Piece>(0.0), std::make_shared<Piece>(1.0),
      std::make_shared<Piece>(2.0), std::make_shared<Piece>(3.0),
      std::make_shared<Piece>(4.0), std::make_shared<Piece>(5.0),
      std::make_shared<Piece>(6.0), std::make_shared<Piece>(7.0),
      std::make_shared<Piece>(8.0), std::make_shared<Piece>(9.0)
  };
  Page p(&ind.front(), &ind.back());
  EXPECT_NEAR(44.22329, p.GetFitness(), 0.00001);
}

TEST(EvaluationTests, TestEvaluateTwoPages) {
  std::vector<std::shared_ptr<Piece>> pieces {
      std::make_shared<Piece>(0.0), std::make_shared<Piece>(1.0),
      std::make_shared<Piece>(2.0), std::make_shared<Piece>(3.0),
      std::make_shared<Piece>(4.0), std::make_shared<Piece>(5.0),
      kPageBreak,
      std::make_shared<Piece>(6.0), std::make_shared<Piece>(7.0),
      std::make_shared<Piece>(8.0), std::make_shared<Piece>(9.0)
  };
  Individual individual(pieces);
  std::vector<Page> pages = individual.GetPages();
  EXPECT_EQ(pages.size(), 2);
  EXPECT_NEAR(21.28217, pages[0].GetFitness(), 0.00001);
  EXPECT_NEAR(6.92820, pages[1].GetFitness(), 0.00001);
}