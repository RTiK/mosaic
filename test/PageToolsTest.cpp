//
// Created by Artem Khatchatourov on 30.01.21.
//

#include <gtest/gtest.h>
#include "PageTools.h"

TEST(PageBreakTest, TestSimplePageBreak) {
    std::vector<std::shared_ptr<Piece>> pieces{
            std::make_shared<Piece>(), std::make_shared<Piece>(),
            std::make_shared<Piece>(), std::make_shared<Piece>()
    };

    std::vector<std::shared_ptr<PageEdge>> pages = splitGeneToPages(pieces);
    EXPECT_EQ(pages.size(), 1);
    EXPECT_EQ(pages[0].get()->first->get(), const_cast<std::shared_ptr<Piece>*>(&pieces.front())->get());
    EXPECT_EQ(pages[0].get()->second->get(), const_cast<std::shared_ptr<Piece>*>(&pieces.back())->get());
}

TEST(PageBreakTest, TestLeadingPageBreak) {
    std::vector<std::shared_ptr<Piece>> pieces{4, std::make_shared<Piece>()};
    pieces.emplace(pieces.begin(), PAGE_BREAK);

    std::vector<std::shared_ptr<PageEdge>> pages = splitGeneToPages(pieces);
    EXPECT_EQ(pages.size(), 1);
    EXPECT_EQ(pages[0].get()->first->get(), const_cast<std::shared_ptr<Piece>*>(&pieces[1])->get());
    EXPECT_EQ(pages[0].get()->second->get(), const_cast<std::shared_ptr<Piece>*>(&pieces[2])->get());
}

TEST(PageBreakTest, TestTrailingPageBreak) {
    std::vector<std::shared_ptr<Piece>> pieces{4, std::make_shared<Piece>()};
    pieces.emplace_back(PAGE_BREAK);

    std::vector<std::shared_ptr<PageEdge>> pages = splitGeneToPages(pieces);
    EXPECT_EQ(pages.size(), 1);
    EXPECT_EQ(pages[0].get()->first->get(), const_cast<std::shared_ptr<Piece>*>(&pieces.front())->get());
    EXPECT_EQ(pages[0].get()->second->get(), const_cast<std::shared_ptr<Piece>*>(&pieces.at(2))->get());
    }

    TEST(PageBreakTest, TestOnePiecePage) {
    std::vector<std::shared_ptr<Piece>> pieces{std::make_shared<Piece>(0)};

    std::vector<std::shared_ptr<PageEdge>> pages = splitGeneToPages(pieces);
    EXPECT_EQ(pages.size(), 1);
    EXPECT_EQ(pages[0].get()->first->get(), const_cast<std::shared_ptr<Piece>*>(&pieces[0])->get());
    EXPECT_EQ(pages[0].get()->second->get(), const_cast<std::shared_ptr<Piece>*>(&pieces[0])->get());
}

TEST(PageBreakTest, TestTwoPage) {
    std::vector<std::shared_ptr<Piece>> pieces {
            std::make_shared<Piece>(), std::make_shared<Piece>(),
            PAGE_BREAK,
            std::make_shared<Piece>(), std::make_shared<Piece>()
    };
    std::vector<std::shared_ptr<PageEdge>> pages = splitGeneToPages(pieces);
    EXPECT_EQ(pages.size(), 2);
    EXPECT_EQ(pages[0].get()->first->get(), const_cast<std::shared_ptr<Piece>*>(&pieces[0])->get());
    EXPECT_EQ(pages[0].get()->second->get(), const_cast<std::shared_ptr<Piece>*>(&pieces[1])->get());
    EXPECT_EQ(pages[1].get()->first->get(), const_cast<std::shared_ptr<Piece>*>(&pieces[3])->get());
    EXPECT_EQ(pages[1].get()->second->get(), const_cast<std::shared_ptr<Piece>*>(&pieces[4])->get());
}

TEST(PageBreakTest, TestTripplePageBreak) {
    std::vector<std::shared_ptr<Piece>> pieces {
            std::make_shared<Piece>(), std::make_shared<Piece>(),
            PAGE_BREAK, PAGE_BREAK, PAGE_BREAK,
            std::make_shared<Piece>(), std::make_shared<Piece>()
    };
    std::vector<std::shared_ptr<PageEdge>> pages = splitGeneToPages(pieces);
    EXPECT_EQ(pages.size(), 2);
    EXPECT_EQ(pages[0].get()->first->get(), const_cast<std::shared_ptr<Piece>*>(&pieces[0])->get());
    EXPECT_EQ(pages[0].get()->second->get(), const_cast<std::shared_ptr<Piece>*>(&pieces[1])->get());
    EXPECT_EQ(pages[1].get()->first->get(), const_cast<std::shared_ptr<Piece>*>(&pieces[5])->get());
    EXPECT_EQ(pages[1].get()->second->get(), const_cast<std::shared_ptr<Piece>*>(&pieces[6])->get());
}

TEST(PageBreakTest, TestPageOverflow) {
    std::vector<std::shared_ptr<Piece>> pieces{
            std::make_shared<Piece>(1.0), std::make_shared<Piece>(1.0),  // 0
            std::make_shared<Piece>(1.0), std::make_shared<Piece>(1.0),  // 2
            std::make_shared<Piece>(1.0), std::make_shared<Piece>(1.0),  // 4
            std::make_shared<Piece>(1.0), std::make_shared<Piece>(1.0),  // 6
            std::make_shared<Piece>(1.0), std::make_shared<Piece>(1.0),  // 8
            std::make_shared<Piece>(1.0), std::make_shared<Piece>(1.0),  // 10
            std::make_shared<Piece>(1.0), std::make_shared<Piece>(1.0),  // 12
            std::make_shared<Piece>(1.0), std::make_shared<Piece>(1.0),  // 14
            std::make_shared<Piece>(1.0), std::make_shared<Piece>(1.0),  // 16
            std::make_shared<Piece>(1.0), std::make_shared<Piece>(1.0),  // 20
            std::make_shared<Piece>(1.0), std::make_shared<Piece>(1.0),  // 22
            std::make_shared<Piece>(1.0), std::make_shared<Piece>(1.0),  // 24
            std::make_shared<Piece>(1.0), std::make_shared<Piece>(1.0),  // new page
            std::make_shared<Piece>(1.0), std::make_shared<Piece>(1.0)   // 2
    };
    auto pages = splitGeneToPages(pieces);
    EXPECT_EQ(pages.size(), 2);
    EXPECT_EQ(pages[0]->second - pages[0]->first, MAX_PIECES_ON_PAGE - 1);  // 23
    EXPECT_EQ(pages[1]->second - pages[1]->first, pieces.size() - MAX_PIECES_ON_PAGE - 1);  // 3
}