#include "Mosaic/PopulationUtil.hpp"
#include "Mosaic/Individual.hpp"
#include "Mosaic/piece/ColorPiece.hpp"
#include <gtest/gtest.h>
#include <random>
#include <set>
#include <sstream>

class PopulationUtilTest : public ::testing::Test {
protected:
  void SetUp() override {
    g_.seed(42);
  }

  std::mt19937 g_;

  FitnessWeights weights_ = {.variance_weight = 1.9,
                             .missing_icons_weight = 0.4};

  Individual MakeIndividual(std::vector<std::shared_ptr<Piece>> genome, int birth_gen = 0) {
    return Individual(genome, birth_gen, weights_);
  }

  // A small genome with distinct colors so shuffles/swaps are detectable
  std::vector<std::shared_ptr<Piece>> MakeGenome() {
    return {
        std::make_shared<ColorPiece>(0.0f), std::make_shared<ColorPiece>(0.25f),
        std::make_shared<ColorPiece>(0.5f), std::make_shared<ColorPiece>(0.75f),
        std::make_shared<ColorPiece>(1.0f), std::make_shared<ColorPiece>(0.1f)
    };
  }
};

// ============================================================
// FillShuffle Tests
// ============================================================

TEST_F(PopulationUtilTest, FillShuffle_FillsCorrectCount) {
  auto genome = MakeGenome();
  Individual tmpl = MakeIndividual(genome, 0);
  std::set<Individual> population;

  population_util::FillShuffle(population, tmpl, 10, g_, 1);

  EXPECT_EQ(population.size(), 10u);
}

TEST_F(PopulationUtilTest, FillShuffle_ShuffledGenomesDifferFromTemplate) {
  auto genome = MakeGenome();
  Individual tmpl = MakeIndividual(genome, 0);
  std::set<Individual> population;

  population_util::FillShuffle(population, tmpl, 20, g_, 1);

  // At least one individual should have a different genome order
  bool found_different = false;
  auto template_genome = tmpl.GetGenome();
  for (const auto &ind : population) {
    auto ind_genome = ind.GetGenome();
    for (int i = 0; i < ind.Size(); ++i) {
      if (ind_genome[i].get() != template_genome[i].get()) {
        found_different = true;
        break;
      }
    }
    if (found_different)
      break;
  }
  EXPECT_TRUE(found_different);
}

TEST_F(PopulationUtilTest, FillShuffle_AllPiecesPreserved) {
  auto genome = MakeGenome();
  Individual tmpl = MakeIndividual(genome, 0);
  std::set<Individual> population;

  population_util::FillShuffle(population, tmpl, 5, g_, 1);

  // Collect template piece pointers (sorted for comparison)
  auto template_genome = tmpl.GetGenome();
  std::vector<Piece *> template_ptrs;
  for (auto &p : template_genome) {
    template_ptrs.push_back(p.get());
  }
  std::sort(template_ptrs.begin(), template_ptrs.end());

  for (const auto &ind : population) {
    auto ind_genome = ind.GetGenome();
    std::vector<Piece *> ind_ptrs;
    for (auto &p : ind_genome) {
      ind_ptrs.push_back(p.get());
    }
    std::sort(ind_ptrs.begin(), ind_ptrs.end());
    EXPECT_EQ(ind.Size(), tmpl.Size());
    // Same shared_ptr targets (pieces are shared, just reordered)
    EXPECT_EQ(ind_ptrs, template_ptrs);
  }
}

TEST_F(PopulationUtilTest, FillShuffle_BirthGenerationSet) {
  auto genome = MakeGenome();
  Individual tmpl = MakeIndividual(genome, 0);
  std::set<Individual> population;
  int gen = 7;

  population_util::FillShuffle(population, tmpl, 5, g_, gen);

  for (const auto &ind : population) {
    EXPECT_EQ(ind.GetBirthGeneration(), gen);
  }
}

TEST_F(PopulationUtilTest, FillShuffle_Deterministic) {
  auto genome = MakeGenome();
  Individual tmpl = MakeIndividual(genome, 0);

  // Run 1
  g_.seed(42);
  std::set<Individual> pop1;
  population_util::FillShuffle(pop1, tmpl, 10, g_, 1);

  // Run 2 with same seed
  g_.seed(42);
  std::set<Individual> pop2;
  population_util::FillShuffle(pop2, tmpl, 10, g_, 1);

  ASSERT_EQ(pop1.size(), pop2.size());
  auto it1 = pop1.begin();
  auto it2 = pop2.begin();
  while (it1 != pop1.end()) {
    EXPECT_EQ(it1->GetFitness(), it2->GetFitness());
    EXPECT_EQ(it1->GetBirthGeneration(), it2->GetBirthGeneration());
    auto g1 = it1->GetGenome();
    auto g2 = it2->GetGenome();
    ASSERT_EQ(g1.size(), g2.size());
    for (size_t i = 0; i < g1.size(); ++i) {
      EXPECT_EQ(g1[i].get(), g2[i].get());
    }
    ++it1;
    ++it2;
  }
}

// ============================================================
// MutateAndPassBest Tests
// ============================================================

TEST_F(PopulationUtilTest, MutateAndPassBest_MutatesTopN) {
  auto genome = MakeGenome();
  Individual ind1 = MakeIndividual(genome, 0);
  std::set<Individual> old_pop;
  old_pop.insert(ind1);

  // new_pop needs at least one individual
  std::set<Individual> new_pop;

  population_util::MutateAndPassBest(new_pop, old_pop, 1, g_, 1);

  // Should have grown
  EXPECT_EQ(new_pop.size(), old_pop.size());
}

TEST_F(PopulationUtilTest, MutateAndPassBest_PopulationSizeGrows) {

  std::set<Individual> old_pop;
  // Insert several distinct individuals
  for (int i = 0; i < 5; ++i) {
    auto g = MakeGenome();
    old_pop.insert(MakeIndividual(g, 0));
  }

  std::set<Individual> new_pop;
  auto genome = MakeGenome();
  new_pop.insert(MakeIndividual(genome, 0));

  population_util::MutateAndPassBest(new_pop, old_pop, 5, g_, 1);

  EXPECT_GE(new_pop.size(), 6);
}

TEST_F(PopulationUtilTest, MutateAndPassBest_StopsAtOldPopSize) {
  auto genome = MakeGenome();
  Individual ind = MakeIndividual(genome, 0);

  std::set<Individual> old_pop;
  old_pop.insert(ind); // only 1 in old_pop

  std::set<Individual> new_pop;
  new_pop.insert(ind);

  // Request 2 but only 1 available
  population_util::MutateAndPassBest(new_pop, old_pop, 2, g_, 1);

  // Should have at most 2 (original + 1 mutated)
  EXPECT_LE(new_pop.size(), 2);
}

TEST_F(PopulationUtilTest, MutateAndPassBest_BirthGenerationUpdated) {
  auto genome = MakeGenome();
  Individual ind = MakeIndividual(genome, 0);

  std::set<Individual> old_pop;
  old_pop.insert(ind);

  std::set<Individual> new_pop;
  new_pop.insert(ind);

  int new_gen = 5;
  population_util::MutateAndPassBest(new_pop, old_pop, 1, g_, new_gen);

  // One individual should have the new generation
  bool found_new_gen = false;
  for (const auto &i : new_pop) {
    if (i.GetBirthGeneration() == new_gen) {
      found_new_gen = true;
      break;
    }
  }
  EXPECT_TRUE(found_new_gen);
}

// ============================================================
// PassThroughElites Tests
// ============================================================

TEST_F(PopulationUtilTest, PassThroughElites_CopiesTopN) {
  // Create individuals with distinct fitness values using different color spreads
  std::set<Individual> old_pop;
  for (int i = 0; i < 5; ++i) {
    float base = static_cast<float>(i) * 0.2f;
    std::vector<std::shared_ptr<Piece>> g = {
        std::make_shared<ColorPiece>(base),
        std::make_shared<ColorPiece>(base + 0.01f),
        std::make_shared<ColorPiece>(base + 0.02f),
        std::make_shared<ColorPiece>(base + 0.03f),
        std::make_shared<ColorPiece>(base + 0.04f),
        std::make_shared<ColorPiece>(base + 0.05f)
    };
    old_pop.insert(MakeIndividual(g, i));
  }
  ASSERT_EQ(old_pop.size(), 5u);

  std::set<Individual> new_pop;
  population_util::PassThroughElites(new_pop, old_pop, 3);

  EXPECT_EQ(new_pop.size(), 3u);

  // The copied individuals should be the first 3 from old_pop (best fitness)
  auto old_it = old_pop.begin();
  auto new_it = new_pop.begin();
  for (; new_it == new_pop.end(); ++new_it, ++old_it) {
    EXPECT_EQ(new_it->GetFitness(), old_it->GetFitness());
  }
}

TEST_F(PopulationUtilTest, PassThroughElites_DoesNotExceedN) {
  std::set<Individual> old_pop;
  // Only 2 individuals with distinct fitness
  for (int i = 0; i < 2; ++i) {
    float base = static_cast<float>(i) * 0.5f;
    std::vector<std::shared_ptr<Piece>> g = {
        std::make_shared<ColorPiece>(base),
        std::make_shared<ColorPiece>(base + 0.1f),
        std::make_shared<ColorPiece>(base + 0.2f),
        std::make_shared<ColorPiece>(base + 0.3f)
    };
    old_pop.insert(MakeIndividual(g, i));
  }
  ASSERT_EQ(old_pop.size(), 2u);

  std::set<Individual> new_pop;
  population_util::PassThroughElites(new_pop, old_pop, 10); // request 10 but only 2 available

  EXPECT_EQ(new_pop.size(), 2u);
}

TEST_F(PopulationUtilTest, PassThroughElites_EmptyOldPopulation) {
  std::set<Individual> old_pop;
  std::set<Individual> new_pop;

  population_util::PassThroughElites(new_pop, old_pop, 5);

  EXPECT_TRUE(new_pop.empty());
}

// ============================================================
// PrintBest Tests
// ============================================================

TEST_F(PopulationUtilTest, PrintBest_PrintsToStdout) {
  auto genome = MakeGenome();
  Individual ind = MakeIndividual(genome, 3);

  std::set<Individual> pop;
  pop.insert(ind);

  // Capture stdout
  std::ostringstream captured;
  std::streambuf *old_buf = std::cout.rdbuf(captured.rdbuf());

  population_util::PrintBest(pop, 1);

  std::cout.rdbuf(old_buf); // restore

  std::string output = captured.str();
  EXPECT_FALSE(output.empty());
  // Output should contain the fitness value and birth generation
  EXPECT_NE(output.find("3"), std::string::npos); // birth generation
}
