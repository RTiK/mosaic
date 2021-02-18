//
// Created by Artem Khatchatourov on 18.02.21.
//

#include "gtest/gtest.h"
#include "Individual.h"


TEST(IndividualConstructorTests, CopyConstructorTest) {
    std::vector<std::shared_ptr<Piece>> genome{
            std::make_shared<Piece>(), std::make_shared<Piece>(),
            std::make_shared<Piece>(), std::make_shared<Piece>()
    };
    Individual original_individual;
    original_individual.genome = genome;
    Individual copy_individual(original_individual);
    EXPECT_NE(&original_individual, &copy_individual);
    EXPECT_NE(&original_individual.genome, &copy_individual.genome);
    EXPECT_EQ(original_individual.fitness, copy_individual.fitness);
}

TEST(IndividualSwapTests, SwapAdjacent) {
    std::vector<std::shared_ptr<Piece>> genome{
        std::make_shared<Piece>(), std::make_shared<Piece>(),
        std::make_shared<Piece>(), std::make_shared<Piece>()
    };
    Individual individual;
    individual.genome = genome;
    unsigned int index1 = 1;
    unsigned int index2 = 2;
    auto ref1 = individual.genome.at(index1).get();
    auto ref2 = individual.genome.at(index2).get();
    individual.swap(index1, index2);
    EXPECT_EQ(ref1, individual.genome.at(index2).get());
    EXPECT_EQ(ref2, individual.genome.at(index1).get());
}

TEST(IndividualSwapTests, SwapEdges) {
    std::vector<std::shared_ptr<Piece>> genome{
            std::make_shared<Piece>(), std::make_shared<Piece>(),
            std::make_shared<Piece>(), std::make_shared<Piece>()
    };
    Individual individual;
    individual.genome = genome;
    unsigned int index1 = 0;
    unsigned int index2 = genome.size()-1;
    auto ref1 = individual.genome.at(index1).get();
    auto ref2 = individual.genome.at(index2).get();
    individual.swap(index1, index2);
    EXPECT_EQ(ref1, individual.genome.at(index2).get());
    EXPECT_EQ(ref2, individual.genome.at(index1).get());
}

TEST(IndividualSwapTests, SwapSameIndex) {
    std::vector<std::shared_ptr<Piece>> genome{
            std::make_shared<Piece>(), std::make_shared<Piece>(),
            std::make_shared<Piece>(), std::make_shared<Piece>()
    };
    Individual individual;
    individual.genome = genome;
    unsigned int index1 = 1;
    auto ref1 = individual.genome.at(index1).get();
    individual.swap(index1, index1);
    EXPECT_EQ(ref1, individual.genome.at(index1).get());
}