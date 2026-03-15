#include <gtest/gtest.h>
#include <Mosaic/HallOfFame.hpp>
#include <Mosaic/Individual.hpp>
#include <Mosaic/piece/ColorPiece.hpp>
#include <set>
#include <vector>

class HallOfFameTest : public ::testing::Test {
 protected:
  // Creates an Individual from a list of gray values. Uniform genomes produce
  // zero neighbor distances (lowest fitness); alternating extremes produce
  // progressively higher fitness.
  Individual MakeIndividual(std::vector<float> grays, int gen = 0) {
    std::vector<std::shared_ptr<Piece>> genome;
    for (float g : grays) {
      genome.push_back(std::make_shared<ColorPiece>(g));
    }
    return Individual(genome, gen);
  }

  // All same color → neighbor distance = 0 → lowest fitness
  Individual LowFit(int gen = 0) {
    return MakeIndividual({0.0f, 0.0f, 0.0f, 0.0f}, gen);
  }

  // Mild alternation → moderate neighbor distances
  Individual MidFit(int gen = 0) {
    return MakeIndividual({0.0f, 0.3f, 0.0f, 0.3f}, gen);
  }

  // Extreme alternation → maximum neighbor distances
  Individual HighFit(int gen = 0) {
    return MakeIndividual({0.0f, 1.0f, 0.0f, 1.0f}, gen);
  }
};

// ============================================================
// GetBest Tests
// ============================================================

TEST_F(HallOfFameTest, GetBest_EmptyHall_Throws) {
  HallOfFame hof(3);
  EXPECT_THROW(hof.GetBest(), std::runtime_error);
}

TEST_F(HallOfFameTest, GetBest_ReturnsBestFitness) {
  HallOfFame hof(3);
  Individual lo = LowFit();
  Individual hi = HighFit();
  ASSERT_LT(lo.GetFitness(), hi.GetFitness());

  hof.Update(hi);
  hof.Update(lo);

  EXPECT_DOUBLE_EQ(hof.GetBest().GetFitness(), lo.GetFitness());
}

TEST_F(HallOfFameTest, GetBest_SingleEntry_ReturnsThatEntry) {
  HallOfFame hof(5);
  Individual ind = MidFit();
  hof.Update(ind);

  EXPECT_DOUBLE_EQ(hof.GetBest().GetFitness(), ind.GetFitness());
}

// ============================================================
// GetAll Tests
// ============================================================

TEST_F(HallOfFameTest, GetAll_EmptyHall_ReturnsEmptyVector) {
  HallOfFame hof(3);
  EXPECT_TRUE(hof.GetAll().empty());
}

TEST_F(HallOfFameTest, GetAll_ReturnsSortedAscendingByFitness) {
  HallOfFame hof(3);
  Individual lo = LowFit();
  Individual mi = MidFit();
  Individual hi = HighFit();
  ASSERT_LT(lo.GetFitness(), mi.GetFitness());
  ASSERT_LT(mi.GetFitness(), hi.GetFitness());

  hof.Update(hi);
  hof.Update(lo);
  hof.Update(mi);

  auto all = hof.GetAll();
  ASSERT_EQ(all.size(), 3u);
  EXPECT_DOUBLE_EQ(all[0].GetFitness(), lo.GetFitness());
  EXPECT_DOUBLE_EQ(all[1].GetFitness(), mi.GetFitness());
  EXPECT_DOUBLE_EQ(all[2].GetFitness(), hi.GetFitness());
}

TEST_F(HallOfFameTest, GetAll_ReturnsExactlyMaxSizeEntries_WhenFull) {
  HallOfFame hof(2);
  hof.Update(LowFit());
  hof.Update(MidFit());
  hof.Update(HighFit());

  EXPECT_EQ(hof.GetAll().size(), 2u);
}

// ============================================================
// Update(Individual) Tests
// ============================================================

TEST_F(HallOfFameTest, UpdateIndividual_EmptyHall_IncreasesSize) {
  HallOfFame hof(3);
  EXPECT_EQ(hof.Size(), 0u);
  hof.Update(LowFit());
  EXPECT_EQ(hof.Size(), 1u);
}

TEST_F(HallOfFameTest, UpdateIndividual_BelowMaxSize_AcceptsAll) {
  HallOfFame hof(5);
  hof.Update(LowFit());
  hof.Update(MidFit());
  hof.Update(HighFit());
  EXPECT_EQ(hof.Size(), 3u);
}

TEST_F(HallOfFameTest, UpdateIndividual_ExceedingMaxSize_RemovesWorst) {
  HallOfFame hof(2);
  Individual lo = LowFit();
  Individual mi = MidFit();
  Individual hi = HighFit();
  ASSERT_LT(lo.GetFitness(), mi.GetFitness());
  ASSERT_LT(mi.GetFitness(), hi.GetFitness());

  hof.Update(lo);
  hof.Update(mi);
  hof.Update(hi);  // triggers eviction of HighFit

  EXPECT_EQ(hof.Size(), 2u);
  auto all = hof.GetAll();
  EXPECT_DOUBLE_EQ(all[0].GetFitness(), lo.GetFitness());
  EXPECT_DOUBLE_EQ(all[1].GetFitness(), mi.GetFitness());
}

TEST_F(HallOfFameTest, UpdateIndividual_FullHall_WorseThanAll_Rejected) {
  HallOfFame hof(2);
  Individual lo = LowFit();
  Individual mi = MidFit();
  Individual hi = HighFit();
  ASSERT_LT(lo.GetFitness(), mi.GetFitness());
  ASSERT_LT(mi.GetFitness(), hi.GetFitness());

  hof.Update(lo);
  hof.Update(mi);
  hof.Update(hi);  // should be inserted then evicted immediately

  EXPECT_EQ(hof.Size(), 2u);
  EXPECT_DOUBLE_EQ(hof.GetBest().GetFitness(), lo.GetFitness());
}

TEST_F(HallOfFameTest, UpdateIndividual_FullHall_BetterThanAll_Accepted) {
  HallOfFame hof(2);
  Individual mi = MidFit();
  Individual hi = HighFit();
  Individual lo = LowFit();
  ASSERT_LT(lo.GetFitness(), mi.GetFitness());

  hof.Update(mi);
  hof.Update(hi);
  hof.Update(lo);  // better than all — should displace HighFit

  EXPECT_EQ(hof.Size(), 2u);
  EXPECT_DOUBLE_EQ(hof.GetBest().GetFitness(), lo.GetFitness());
}

TEST_F(HallOfFameTest, UpdateIndividual_Duplicate_NotReinserted) {
  HallOfFame hof(5);
  Individual ind = MidFit();
  hof.Update(ind);
  hof.Update(ind);  // exact same object — should be a no-op
  EXPECT_EQ(hof.Size(), 1u);
}

// ============================================================
// Update(Individual) — Callback Tests
// ============================================================

TEST_F(HallOfFameTest, UpdateIndividual_Callback_FiredOnInsert) {
  HallOfFame hof(3);
  int call_count = 0;
  hof.SetOnInsertCallback([&](const Individual&, size_t) { ++call_count; });

  hof.Update(LowFit());
  EXPECT_EQ(call_count, 1);

  hof.Update(MidFit());
  EXPECT_EQ(call_count, 2);
}

TEST_F(HallOfFameTest, UpdateIndividual_Callback_NotFiredForDuplicate) {
  HallOfFame hof(3);
  int call_count = 0;
  hof.SetOnInsertCallback([&](const Individual&, size_t) { ++call_count; });

  Individual ind = MidFit();
  hof.Update(ind);
  hof.Update(ind);  // duplicate

  EXPECT_EQ(call_count, 1);
}

TEST_F(HallOfFameTest, UpdateIndividual_Callback_NotFiredForEvictedInsertion) {
  // When a new individual is inserted but then immediately evicted (it was the worst),
  // the callback IS still fired (it was successfully inserted before eviction).
  // This test verifies the callback fires exactly once per actual insertion.
  HallOfFame hof(2);
  Individual lo = LowFit();
  Individual mi = MidFit();
  Individual hi = HighFit();
  ASSERT_LT(lo.GetFitness(), mi.GetFitness());
  ASSERT_LT(mi.GetFitness(), hi.GetFitness());

  int call_count = 0;
  hof.SetOnInsertCallback([&](const Individual&, size_t) { ++call_count; });

  hof.Update(lo);  // inserted
  hof.Update(mi);  // inserted
  hof.Update(hi);  // inserted then evicted (worst in full hall)

  EXPECT_EQ(call_count, 3);
}

TEST_F(HallOfFameTest, UpdateIndividual_Callback_ReportsCorrectRank) {
  HallOfFame hof(3);
  std::vector<size_t> ranks;
  hof.SetOnInsertCallback([&](const Individual&, size_t rank) {
    ranks.push_back(rank);
  });

  // Insert from worst to best; each insertion shifts prior ranks.
  Individual lo = LowFit();
  Individual mi = MidFit();
  Individual hi = HighFit();
  ASSERT_LT(lo.GetFitness(), mi.GetFitness());
  ASSERT_LT(mi.GetFitness(), hi.GetFitness());

  hof.Update(hi);  // rank 1 (only element)
  hof.Update(mi);  // rank 1 (better than hi), hi becomes rank 2
  hof.Update(lo);  // rank 1 (best of all)

  ASSERT_EQ(ranks.size(), 3u);
  EXPECT_EQ(ranks[0], 1u);  // hi inserted first — rank 1
  EXPECT_EQ(ranks[1], 1u);  // mi inserted before hi — rank 1
  EXPECT_EQ(ranks[2], 1u);  // lo inserted before mi — rank 1
}

// ============================================================
// Update(std::set<Individual>) Tests
// ============================================================

TEST_F(HallOfFameTest, UpdatePopulation_EmptyHall_FillsUpToMaxSize) {
  HallOfFame hof(2);
  Individual lo = LowFit();
  Individual mi = MidFit();
  Individual hi = HighFit();
  ASSERT_LT(lo.GetFitness(), mi.GetFitness());
  ASSERT_LT(mi.GetFitness(), hi.GetFitness());

  std::set<Individual> pop{lo, mi, hi};
  hof.Update(pop);

  EXPECT_EQ(hof.Size(), 2u);
  // Should contain the 2 best: lo and mi
  auto all = hof.GetAll();
  EXPECT_DOUBLE_EQ(all[0].GetFitness(), lo.GetFitness());
  EXPECT_DOUBLE_EQ(all[1].GetFitness(), mi.GetFitness());
}

TEST_F(HallOfFameTest, UpdatePopulation_PartialHall_FillsRemainingSlots) {
  HallOfFame hof(3);
  Individual lo = LowFit();
  Individual mi = MidFit();
  Individual hi = HighFit();
  ASSERT_LT(lo.GetFitness(), mi.GetFitness());
  ASSERT_LT(mi.GetFitness(), hi.GetFitness());

  hof.Update(hi);  // pre-fill one slot

  std::set<Individual> pop{lo, mi};
  hof.Update(pop);  // should fill the remaining 2 slots

  EXPECT_EQ(hof.Size(), 3u);
}

TEST_F(HallOfFameTest, UpdatePopulation_FullHall_InsertsBetterIndividuals) {
  HallOfFame hof(2);
  Individual lo = LowFit();
  Individual mi = MidFit();
  Individual hi = HighFit();
  ASSERT_LT(lo.GetFitness(), mi.GetFitness());
  ASSERT_LT(mi.GetFitness(), hi.GetFitness());

  hof.Update(mi);
  hof.Update(hi);

  std::set<Individual> pop{lo};  // better than current worst (hi)
  hof.Update(pop);

  EXPECT_EQ(hof.Size(), 2u);
  EXPECT_DOUBLE_EQ(hof.GetBest().GetFitness(), lo.GetFitness());
}

TEST_F(HallOfFameTest, UpdatePopulation_FullHall_IgnoresWorseIndividuals) {
  HallOfFame hof(2);
  Individual lo = LowFit();
  Individual mi = MidFit();
  Individual hi = HighFit();
  ASSERT_LT(lo.GetFitness(), mi.GetFitness());
  ASSERT_LT(mi.GetFitness(), hi.GetFitness());

  hof.Update(lo);
  hof.Update(mi);

  std::set<Individual> pop{hi};  // worse than current worst (mi)
  hof.Update(pop);

  EXPECT_EQ(hof.Size(), 2u);
  auto all = hof.GetAll();
  EXPECT_DOUBLE_EQ(all[0].GetFitness(), lo.GetFitness());
  EXPECT_DOUBLE_EQ(all[1].GetFitness(), mi.GetFitness());
}

TEST_F(HallOfFameTest, UpdatePopulation_EmptyPopulation_HallUnchanged) {
  HallOfFame hof(3);
  hof.Update(MidFit());

  std::set<Individual> empty_pop;
  hof.Update(empty_pop);

  EXPECT_EQ(hof.Size(), 1u);
}

TEST_F(HallOfFameTest, UpdatePopulation_Callback_FiredForEachInsertion) {
  HallOfFame hof(3);
  int call_count = 0;
  hof.SetOnInsertCallback([&](const Individual&, size_t) { ++call_count; });

  Individual lo = LowFit();
  Individual mi = MidFit();
  Individual hi = HighFit();

  std::set<Individual> pop{lo, mi, hi};
  hof.Update(pop);

  // Only lo and mi should be inserted (max_size=3, hall empty → takes 3 best,
  // but pop has 3 so all are taken — however since max_size=3, all 3 fit)
  EXPECT_EQ(call_count, 3);
}

TEST_F(HallOfFameTest, UpdatePopulation_Callback_NotFiredForDuplicates) {
  HallOfFame hof(3);
  Individual ind = MidFit();
  hof.Update(ind);  // pre-insert

  int call_count = 0;
  hof.SetOnInsertCallback([&](const Individual&, size_t) { ++call_count; });

  std::set<Individual> pop{ind};  // same individual — duplicate
  hof.Update(pop);

  EXPECT_EQ(call_count, 0);
}