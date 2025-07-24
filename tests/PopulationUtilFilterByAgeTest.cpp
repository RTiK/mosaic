#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <set>
#include <random>
#include "Mosaic/PopulationUtil.hpp"
#include "Mosaic/IndividualGeneration.hpp"

class PopulationUtilFilterByAgeTest : public ::testing::Test {
protected:
    void SetUp() override {
        g.seed(42); // Fixed seed for reproducible tests
    }

    std::mt19937 g;
    
    // Helper function to create individuals with specific birth generations
    Individual CreateIndividualWithBirthGeneration(int birth_generation) {
        return individual_generation::GenerateIndividualGrayRandom(10, 1, g, birth_generation);
    }
};

TEST_F(PopulationUtilFilterByAgeTest, EmptyPopulation) {
    std::set<Individual> population;
    
    population_util::FilterByAge(population, 100, 50);
    
    EXPECT_TRUE(population.empty());
}

TEST_F(PopulationUtilFilterByAgeTest, AllIndividualsWithinAgeLimit) {
    std::set<Individual> population;
    
    // Add individuals born in generations 95, 96, 97, 98, 99
    for (int birth_gen = 95; birth_gen <= 99; ++birth_gen) {
        population.insert(CreateIndividualWithBirthGeneration(birth_gen));
    }
    
    size_t original_size = population.size();
    
    // Current generation 100, max age 50 -> all should survive (ages 1-5)
    population_util::FilterByAge(population, 100, 50);
    
    EXPECT_EQ(population.size(), original_size);
}

TEST_F(PopulationUtilFilterByAgeTest, AllIndividualsExceedAgeLimit) {
    std::set<Individual> population;
    
    // Add individuals born in generations 40, 30, 20, 10
    for (int birth_gen : {40, 30, 20, 10}) {
        population.insert(CreateIndividualWithBirthGeneration(birth_gen));
    }
    
    // Current generation 100, max age 50 -> all should be removed (ages 60, 70, 80, 90)
    population_util::FilterByAge(population, 100, 50);
    
    EXPECT_TRUE(population.empty());
}

TEST_F(PopulationUtilFilterByAgeTest, MixedAges) {
    std::set<Individual> population;
    
    // Add individuals with mixed ages
    std::vector<int> birth_generations = {30, 60, 70, 80, 90, 95};
    for (int birth_gen : birth_generations) {
        population.insert(CreateIndividualWithBirthGeneration(birth_gen));
    }
    
    // Current generation 100, max age 50
    // Ages will be: 70, 40, 30, 20, 10, 5
    // Should keep: birth_gen 60, 70, 80, 90, 95 (ages 40, 30, 20, 10, 5)
    // Should remove: birth_gen 30 (age 70)
    
    population_util::FilterByAge(population, 100, 50);
    
    EXPECT_EQ(population.size(), 5);
    
    // Verify that the remaining individuals have acceptable ages
    for (const auto& individual : population) {
        int age = 100 - individual.GetBirthGeneration();
        EXPECT_LE(age, 50);
    }
}

TEST_F(PopulationUtilFilterByAgeTest, ExactAgeLimit) {
    std::set<Individual> population;
    
    // Add individuals with ages exactly at and around the limit
    population.insert(CreateIndividualWithBirthGeneration(50));  // age 50 (at limit)
    population.insert(CreateIndividualWithBirthGeneration(49));  // age 51 (exceeds limit)
    population.insert(CreateIndividualWithBirthGeneration(51));  // age 49 (within limit)
    
    population_util::FilterByAge(population, 100, 50);
    
    EXPECT_EQ(population.size(), 2);
    
    // Verify remaining individuals
    std::vector<int> remaining_birth_gens;
    for (const auto& individual : population) {
        remaining_birth_gens.push_back(individual.GetBirthGeneration());
    }
    
    EXPECT_THAT(remaining_birth_gens, ::testing::UnorderedElementsAre(50, 51));
}

TEST_F(PopulationUtilFilterByAgeTest, ZeroMaxAge) {
    std::set<Individual> population;
    
    // Add individuals from different generations
    population.insert(CreateIndividualWithBirthGeneration(100)); // age 0
    population.insert(CreateIndividualWithBirthGeneration(99));  // age 1
    population.insert(CreateIndividualWithBirthGeneration(95));  // age 5
    
    // Max age 0 means only current generation individuals survive
    population_util::FilterByAge(population, 100, 0);
    
    EXPECT_EQ(population.size(), 1);
    EXPECT_EQ(population.begin()->GetBirthGeneration(), 100);
}

TEST_F(PopulationUtilFilterByAgeTest, NegativeAgeCalculation) {
    std::set<Individual> population;
    
    // Add individual born in future generation (edge case)
    population.insert(CreateIndividualWithBirthGeneration(105)); // age -5
    population.insert(CreateIndividualWithBirthGeneration(100)); // age 0
    population.insert(CreateIndividualWithBirthGeneration(95));  // age 5
    
    population_util::FilterByAge(population, 100, 10);
    
    // All should survive (negative age is <= max_age)
    EXPECT_EQ(population.size(), 3);
}

TEST_F(PopulationUtilFilterByAgeTest, LargePopulation) {
    std::set<Individual> population;
    
    // Create a larger population with various ages
    for (int birth_gen = 0; birth_gen <= 100; birth_gen += 10) {
        population.insert(CreateIndividualWithBirthGeneration(birth_gen));
    }
    
    size_t original_size = population.size(); // Should be 11 individuals
    
    // Current generation 100, max age 30
    // Should keep birth_gen: 70, 80, 90, 100 (ages 30, 20, 10, 0)
    population_util::FilterByAge(population, 100, 30);
    
    EXPECT_EQ(population.size(), 4);
    EXPECT_LT(population.size(), original_size);
    
    // Verify all remaining individuals are within age limit
    for (const auto& individual : population) {
        int age = 100 - individual.GetBirthGeneration();
        EXPECT_LE(age, 30);
        EXPECT_GE(individual.GetBirthGeneration(), 70);
    }
}

TEST_F(PopulationUtilFilterByAgeTest, SingleIndividual) {
    std::set<Individual> population;
    
    // Test with single individual that should be removed
    population.insert(CreateIndividualWithBirthGeneration(40)); // age 60
    
    population_util::FilterByAge(population, 100, 50);
    
    EXPECT_TRUE(population.empty());
    
    // Test with single individual that should remain
    population.insert(CreateIndividualWithBirthGeneration(80)); // age 20
    
    population_util::FilterByAge(population, 100, 50);
    
    EXPECT_EQ(population.size(), 1);
    EXPECT_EQ(population.begin()->GetBirthGeneration(), 80);
}
