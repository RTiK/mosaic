//
// Created by Artem Khatchatourov on 18.02.21.
//

#include "gtest/gtest.h"
#include "Individual.h"

TEST(IndividualConstructorTests, CopyConstructorTest) {
  std::vector<std::shared_ptr<Piece>> genome
      {std::make_shared<Piece>(), std::make_shared<Piece>(), std::make_shared<Piece>(), std::make_shared<Piece>()};
  Individual original_individual;
  original_individual.genome_ = genome;
  Individual copy_individual(original_individual);
  EXPECT_NE(&original_individual, &copy_individual);
  EXPECT_NE(&original_individual.genome_, &copy_individual.genome_);
  EXPECT_EQ(original_individual.fitness_, copy_individual.fitness_);
}

TEST(IndividualSwapTests, SwapAdjacent) {
  std::vector<std::shared_ptr<Piece>> genome
      {std::make_shared<Piece>(), std::make_shared<Piece>(), std::make_shared<Piece>(), std::make_shared<Piece>()};
  Individual individual;
  individual.genome_ = genome;
  unsigned int index_1 = 1;
  unsigned int index_2 = 2;
  auto ref_1 = individual.genome_.at(index_1).get();
  auto ref_2 = individual.genome_.at(index_2).get();
  individual.Swap(index_1, index_2);
  EXPECT_EQ(ref_1, individual.genome_.at(index_2).get());
  EXPECT_EQ(ref_2, individual.genome_.at(index_1).get());
}

TEST(IndividualSwapTests, SwapEdges) {
  std::vector<std::shared_ptr<Piece>> genome
      {std::make_shared<Piece>(), std::make_shared<Piece>(), std::make_shared<Piece>(), std::make_shared<Piece>()};
  Individual individual;
  individual.genome_ = genome;
  unsigned int index_1 = 0;
  unsigned int index_2 = genome.size() - 1;
  auto ref_1 = individual.genome_.at(index_1).get();
  auto ref_2 = individual.genome_.at(index_2).get();
  individual.Swap(index_1, index_2);
  EXPECT_EQ(ref_1, individual.genome_.at(index_2).get());
  EXPECT_EQ(ref_2, individual.genome_.at(index_1).get());
}

TEST(IndividualSwapTests, SwapSameIndex) {
  std::vector<std::shared_ptr<Piece>> genome
      {std::make_shared<Piece>(), std::make_shared<Piece>(), std::make_shared<Piece>(), std::make_shared<Piece>()};
  Individual individual;
  individual.genome_ = genome;
  unsigned int index_1 = 1;
  auto ref_1 = individual.genome_.at(index_1).get();
  individual.Swap(index_1, index_1);
  EXPECT_EQ(ref_1, individual.genome_.at(index_1).get());
}