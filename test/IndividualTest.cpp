//
// Created by Artem Khatchatourov on 18.02.21.
//

#include "gtest/gtest.h"
#include "Individual.h"


TEST(IndividualConstructorTests, CopyConstructorTest) {
  std::vector<std::shared_ptr<Piece>> genome {
    std::make_shared<Piece>(), std::make_shared<Piece>(), std::make_shared<Piece>(), std::make_shared<Piece>()
  };
  Individual original_individual(genome);
  Individual copy_individual(original_individual);
  EXPECT_NE(&original_individual, &copy_individual);
  EXPECT_EQ(original_individual.GetGenome(), copy_individual.GetGenome());
  EXPECT_EQ(original_individual.GetFitness(), copy_individual.GetFitness());
}

/**
 * Tests the individual's swap method.
 */
TEST(IndividualSwapTests, SwapAdjacent) {
  std::vector<std::shared_ptr<Piece>> genome {
    std::make_shared<Piece>(), std::make_shared<Piece>(), std::make_shared<Piece>(), std::make_shared<Piece>()
  };
  Individual individual(genome);
  unsigned int index_1 = 1;
  unsigned int index_2 = 2;
  auto ref_1 = individual.GetGenome().at(index_1).get();
  auto ref_2 = individual.GetGenome().at(index_2).get();
  individual.Swap(index_1, index_2);
  EXPECT_EQ(ref_1, individual.GetGenome().at(index_2).get());
  EXPECT_EQ(ref_2, individual.GetGenome().at(index_1).get());
}

TEST(IndividualSwapTests, SwapEdges) {
  std::vector<std::shared_ptr<Piece>> genome {
    std::make_shared<Piece>(), std::make_shared<Piece>(), std::make_shared<Piece>(), std::make_shared<Piece>()
  };
  Individual individual(genome);
  unsigned int index_1 = 0;
  unsigned int index_2 = genome.size() - 1;
  auto ref_1 = individual.GetGenome().at(index_1).get();
  auto ref_2 = individual.GetGenome().at(index_2).get();
  individual.Swap(index_1, index_2);
  EXPECT_EQ(ref_1, individual.GetGenome().at(index_2).get());
  EXPECT_EQ(ref_2, individual.GetGenome().at(index_1).get());
}

TEST(IndividualSwapTests, SwapSameIndex) {
  std::vector<std::shared_ptr<Piece>> genome {
    std::make_shared<Piece>(), std::make_shared<Piece>(),
    std::make_shared<Piece>(), std::make_shared<Piece>()
  };
  Individual individual(genome);
  unsigned int index_1 = 1;
  auto ref_1 = individual.GetGenome().at(index_1).get();
  individual.Swap(index_1, index_1);
  EXPECT_EQ(ref_1, individual.GetGenome().at(index_1).get());
}

TEST(PageBreakTest, TestSimplePageBreak) {
  std::vector<std::shared_ptr<Piece>> pieces {
      std::make_shared<Piece>(), std::make_shared<Piece>(),
      std::make_shared<Piece>(), std::make_shared<Piece>()
  };
  std::vector<Page> pages = Individual::SplitGenomeIntoPages(pieces);
  EXPECT_EQ(pages.size(), 1);
  EXPECT_EQ(pages[0].GetFirstPiece(), &pieces.front());
  EXPECT_EQ(pages[0].GetLastPiece(), &pieces.back());
}

TEST(PageBreakTest, TestLeadingPageBreak) {
  std::vector<std::shared_ptr<Piece>> pieces {
    std::make_shared<Piece>(), std::make_shared<Piece>()
  };
  pieces.emplace(pieces.begin(), kPageBreak);
  std::vector<Page> pages = Individual::SplitGenomeIntoPages(pieces);
  EXPECT_EQ(pages.size(), 1);
  EXPECT_EQ(pages[0].GetFirstPiece(), &pieces[1]);
  EXPECT_EQ(pages[0].GetLastPiece(), &pieces[2]);
}

TEST(PageBreakTest, TestTrailingPageBreak) {
  std::vector<std::shared_ptr<Piece>> pieces {
    std::make_shared<Piece>(), std::make_shared<Piece>(),
    std::make_shared<Piece>(), std::make_shared<Piece>()
  };
  pieces.emplace_back(kPageBreak);
  std::vector<Page> pages = Individual::SplitGenomeIntoPages(pieces);
  EXPECT_EQ(pages.size(), 1);
  EXPECT_EQ(pages[0].GetFirstPiece(), &pieces.front());
  EXPECT_EQ(pages[0].GetLastPiece(), &pieces.at(3));
}

TEST(PageBreakTest, TestOnePiecePage) {
  std::vector<std::shared_ptr<Piece>> pieces {
    std::make_shared<Piece>()
  };
  std::vector<Page> pages = Individual::SplitGenomeIntoPages(pieces);
  EXPECT_EQ(pages.size(), 1);
  EXPECT_EQ(pages[0].GetFirstPiece(), &pieces[0]);
  EXPECT_EQ(pages[0].GetLastPiece(), &pieces[0]);
}

TEST(PageBreakTest, TestTwoPage) {
  std::vector<std::shared_ptr<Piece>> pieces {
      std::make_shared<Piece>(), std::make_shared<Piece>(),
      kPageBreak,
      std::make_shared<Piece>(), std::make_shared<Piece>()
  };
  std::vector<Page> pages = Individual::SplitGenomeIntoPages(pieces);
  EXPECT_EQ(pages.size(), 2);
  EXPECT_EQ(pages[0].GetFirstPiece(), &pieces[0]);
  EXPECT_EQ(pages[0].GetLastPiece(), &pieces[1]);
  EXPECT_EQ(pages[1].GetFirstPiece(), &pieces[3]);
  EXPECT_EQ(pages[1].GetLastPiece(), &pieces[4]);
}

TEST(PageBreakTest, TestTripplePageBreak) {
  std::vector<std::shared_ptr<Piece>> pieces {
      std::make_shared<Piece>(), std::make_shared<Piece>(),
      kPageBreak, kPageBreak, kPageBreak,
      std::make_shared<Piece>(), std::make_shared<Piece>()
  };
  std::vector<Page> pages = Individual::SplitGenomeIntoPages(pieces);
  EXPECT_EQ(pages.size(), 2);
  EXPECT_EQ(pages[0].GetFirstPiece(), &pieces[0]);
  EXPECT_EQ(pages[0].GetLastPiece(), &pieces[1]);
  EXPECT_EQ(pages[1].GetFirstPiece(), &pieces[5]);
  EXPECT_EQ(pages[1].GetLastPiece(), &pieces[6]);
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
      std::make_shared<Piece>(1.0), std::make_shared<Piece>(1.0),  // 0 - new page
      std::make_shared<Piece>(1.0), std::make_shared<Piece>(1.0)   // 2
  };
  std::vector<Page> pages = Individual::SplitGenomeIntoPages(pieces);
  EXPECT_EQ(pages.size(), 2);
  EXPECT_EQ(pages[0].GetLastPiece() - pages[0].GetFirstPiece(), Page::max_pieces_ - 1);
  EXPECT_EQ(pages[1].GetLastPiece() - pages[1].GetFirstPiece(), pieces.size() - Page::max_pieces_ - 1);
}
