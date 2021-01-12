//
// Created by Artem Khatchatourov on 24.10.20.
//

#include <gtest/gtest.h>
#include "Individual.h"


// *ooo
TEST(CalculateNeighborsTests, TestUpperLeft) {
    std::vector<std::shared_ptr<Piece>> ind({std::make_shared<Piece>()});
    auto neighbors = Individual::calculateNeighbors(&ind.front(), &ind.front(), &ind.back());
    EXPECT_EQ(neighbors & (Neighbours::SW | Neighbours::W | Neighbours::NW | Neighbours::N | Neighbours::NE),
              Neighbours::NONE);
}

// o*oo
TEST(CalculateNeighborsTests, TestUpperNextToLeft) {
    std::vector<std::shared_ptr<Piece>> ind(2, std::make_shared<Piece>());
    auto neighbors = Individual::calculateNeighbors(&ind.back(), &ind.front(), &ind.back());
    EXPECT_EQ(neighbors & (Neighbours::NW | Neighbours::N | Neighbours::NE), Neighbours::NONE);
    EXPECT_EQ(neighbors & Neighbours::W, Neighbours::W);
    EXPECT_EQ(neighbors & Neighbours::E, Neighbours::NONE);
}

// ooo*
TEST(CalculateNeighborsTests, TestUpperRight) {
    std::vector<std::shared_ptr<Piece>> ind(4, std::make_shared<Piece>());
    auto neighbors = Individual::calculateNeighbors(&ind.back(), &ind.front(), &ind.back());
    EXPECT_EQ(neighbors & (Neighbours::NW | Neighbours::N | Neighbours::NE | Neighbours::E | Neighbours::SE),
              Neighbours::NONE);
    EXPECT_EQ(neighbors & Neighbours::W, Neighbours::W);
}

// oooo
// *
TEST(CalculateNeighborsTests, TestMiddleLeft) {
    std::vector<std::shared_ptr<Piece>> ind(5, std::make_shared<Piece>());
    auto neighbors = Individual::calculateNeighbors(&ind.back(), &ind.front(), &ind.back());
    EXPECT_EQ(neighbors & (Neighbours::SW | Neighbours::W | Neighbours::NW), Neighbours::NONE);
    EXPECT_EQ(neighbors & (Neighbours::N | Neighbours::NE), Neighbours::N | Neighbours::NE);
}

// oooo
// ooo*
TEST(CalculateNeighborsTests, TestMiddleRight) {
    std::vector<std::shared_ptr<Piece>> ind(8, std::make_shared<Piece>());
    auto neighbors = Individual::calculateNeighbors(&ind.back(), &ind.front(), &ind.back());
    EXPECT_EQ(neighbors & (Neighbours::NE | Neighbours::E | Neighbours::SE), Neighbours::NONE);
    EXPECT_EQ(neighbors & (Neighbours::W | Neighbours::NW | Neighbours::N),
              Neighbours::W | Neighbours::NW | Neighbours::N);
}

// oooo
// o*oo
// ooo
TEST(CalculateNeighborsTests, TestCenter) {
    std::vector<std::shared_ptr<Piece>> ind(13, std::make_shared<Piece>());
    auto neighbors = Individual::calculateNeighbors(&ind.at(5), &ind.front(), &ind.back());
    EXPECT_EQ(neighbors, Neighbours::ALL);
}

// oooo
// o*..
// ....
TEST(CalculateNeighborsTests, TestLowerLast) {
    std::vector<std::shared_ptr<Piece>> ind(6, std::make_shared<Piece>());
    auto neighbors = Individual::calculateNeighbors(&ind.back(), &ind.front(), &ind.back());
    EXPECT_EQ(neighbors & (Neighbours::E | Neighbours::SE | Neighbours::S | Neighbours::SW), Neighbours::NONE);
    EXPECT_EQ(neighbors & Neighbours::N, Neighbours::N);
}

// oooo
// o*o.
// ....
TEST(CalculateNeighborsTests, TestLowerNextToLast) {
    std::vector<std::shared_ptr<Piece>> ind(7, std::make_shared<Piece>());
    auto neighbors = Individual::calculateNeighbors(&ind.at(5), &ind.front(), &ind.back());
    EXPECT_EQ(neighbors & (Neighbours::SE | Neighbours::S | Neighbours::SW), Neighbours::NONE);
}

// oooo
// oo*o
// oo..
TEST(CalculateNeighborsTests, TestLowerPartyObstructed) {
    std::vector<std::shared_ptr<Piece>> ind(10, std::make_shared<Piece>());
    auto neighbor = Individual::calculateNeighbors(&ind.at(6), &ind.front(), &ind.back());
    EXPECT_EQ(neighbor & Neighbours::SW, Neighbours::SW);
    EXPECT_EQ(neighbor & (Neighbours::SE | Neighbours::S), Neighbours::NONE);
}

// oooo
// o*oo
// oo..
TEST(CalculateNeighborsTests, TestLowerObstructed) {
    std::vector<std::shared_ptr<Piece>> ind(10, std::make_shared<Piece>());
    auto neighbor = Individual::calculateNeighbors(&ind.at(5), &ind.front(), &ind.back());
    EXPECT_EQ(neighbor & (Neighbours::S | Neighbours::SW), Neighbours::S | Neighbours::SW);
    EXPECT_EQ(neighbor & Neighbours::SE, Neighbours::NONE);
}

// oooo
// oo*o
// o...
TEST(CalculateNeighborsTests, TestLowerUnobstructed) {
    std::vector<std::shared_ptr<Piece>> ind(9, std::make_shared<Piece>());
    auto neighbor = Individual::calculateNeighbors(&ind.at(6), &ind.front(), &ind.back());
    EXPECT_EQ(neighbor & (Neighbours::SE | Neighbours::S | Neighbours::SW), Neighbours::NONE);
}

// ------
TEST(EvaluationTests, TestEvaluateSimple) {
    std::vector<std::shared_ptr<Piece>> ind{
        std::make_shared<Piece>(0.0), std::make_shared<Piece>(1.0),
        std::make_shared<Piece>(2.0), std::make_shared<Piece>(3.0),
        std::make_shared<Piece>(4.0), std::make_shared<Piece>(5.0),
        std::make_shared<Piece>(6.0), std::make_shared<Piece>(7.0),
        std::make_shared<Piece>(8.0), std::make_shared<Piece>(9.0)
    };
    double a = Individual::evaluate_page(PageEdge{&ind.front(), &ind.back()});
    EXPECT_NEAR(44.22329, a, 0.00001);
}

// ------
TEST(PageBreakTest, TestSimplePageBreak) {
    std::vector<std::shared_ptr<Piece>> pieces{
        std::make_shared<Piece>(), std::make_shared<Piece>(),
        std::make_shared<Piece>(), std::make_shared<Piece>()
    };

    std::vector<std::shared_ptr<PageEdge>> pages = Individual::splitGeneToPages(pieces);
    EXPECT_EQ(pages.size(), 1);
    EXPECT_EQ(pages[0].get()->first->get(), const_cast<std::shared_ptr<Piece>*>(&pieces.front())->get());
    EXPECT_EQ(pages[0].get()->second->get(), const_cast<std::shared_ptr<Piece>*>(&pieces.back())->get());
}

TEST(PageBreakTest, TestLeadingPageBreak) {
    std::vector<std::shared_ptr<Piece>> pieces{4, std::make_shared<Piece>()};
    pieces.emplace(pieces.begin(), Individual::PAGE_BREAK);

    std::vector<std::shared_ptr<PageEdge>> pages = Individual::splitGeneToPages(pieces);
    EXPECT_EQ(pages.size(), 1);
    EXPECT_EQ(pages[0].get()->first->get(), const_cast<std::shared_ptr<Piece>*>(&pieces[1])->get());
    EXPECT_EQ(pages[0].get()->second->get(), const_cast<std::shared_ptr<Piece>*>(&pieces[2])->get());
}

TEST(PageBreakTest, TestTrailingPageBreak) {
    std::vector<std::shared_ptr<Piece>> pieces{4, std::make_shared<Piece>()};
    pieces.emplace_back(Individual::PAGE_BREAK);

    std::vector<std::shared_ptr<PageEdge>> pages = Individual::splitGeneToPages(pieces);
    EXPECT_EQ(pages.size(), 1);
    EXPECT_EQ(pages[0].get()->first->get(), const_cast<std::shared_ptr<Piece>*>(&pieces.front())->get());
    EXPECT_EQ(pages[0].get()->second->get(), const_cast<std::shared_ptr<Piece>*>(&pieces.at(2))->get());
}

TEST(PageBreakTest, TestOnePiecePage) {
    std::vector<std::shared_ptr<Piece>> pieces{std::make_shared<Piece>(0)};

    std::vector<std::shared_ptr<PageEdge>> pages = Individual::splitGeneToPages(pieces);
    EXPECT_EQ(pages.size(), 1);
    EXPECT_EQ(pages[0].get()->first->get(), const_cast<std::shared_ptr<Piece>*>(&pieces[0])->get());
    EXPECT_EQ(pages[0].get()->second->get(), const_cast<std::shared_ptr<Piece>*>(&pieces[0])->get());
}

TEST(PageBreakTest, TestTwoPage) {
    std::vector<std::shared_ptr<Piece>> pieces {
        std::make_shared<Piece>(), std::make_shared<Piece>(),
        Individual::PAGE_BREAK,
        std::make_shared<Piece>(), std::make_shared<Piece>()
    };
    std::vector<std::shared_ptr<PageEdge>> pages = Individual::splitGeneToPages(pieces);
    EXPECT_EQ(pages.size(), 2);
    EXPECT_EQ(pages[0].get()->first->get(), const_cast<std::shared_ptr<Piece>*>(&pieces[0])->get());
    EXPECT_EQ(pages[0].get()->second->get(), const_cast<std::shared_ptr<Piece>*>(&pieces[1])->get());
    EXPECT_EQ(pages[1].get()->first->get(), const_cast<std::shared_ptr<Piece>*>(&pieces[3])->get());
    EXPECT_EQ(pages[1].get()->second->get(), const_cast<std::shared_ptr<Piece>*>(&pieces[4])->get());
}

TEST(PageBreakTest, TestTripplePageBreak) {
    std::vector<std::shared_ptr<Piece>> pieces {
            std::make_shared<Piece>(), std::make_shared<Piece>(),
            Individual::PAGE_BREAK, Individual::PAGE_BREAK, Individual::PAGE_BREAK,
            std::make_shared<Piece>(), std::make_shared<Piece>()
    };
    std::vector<std::shared_ptr<PageEdge>> pages = Individual::splitGeneToPages(pieces);
    EXPECT_EQ(pages.size(), 2);
    EXPECT_EQ(pages[0].get()->first->get(), const_cast<std::shared_ptr<Piece>*>(&pieces[0])->get());
    EXPECT_EQ(pages[0].get()->second->get(), const_cast<std::shared_ptr<Piece>*>(&pieces[1])->get());
    EXPECT_EQ(pages[1].get()->first->get(), const_cast<std::shared_ptr<Piece>*>(&pieces[5])->get());
    EXPECT_EQ(pages[1].get()->second->get(), const_cast<std::shared_ptr<Piece>*>(&pieces[6])->get());
}