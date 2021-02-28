//
// Created by Artem Khatchatourov on 15.02.21.
//

#include "Individual.h"

Individual::Individual() = default;

Individual::Individual(const Individual &ind) {
  genome_ = std::vector(ind.genome_);
  fitness_ = ind.fitness_;
}

Individual::Individual(const Individual &ind, std::mt19937 &g) {
  genome_ = std::vector(ind.genome_);
  std::shuffle(genome_.begin(), genome_.end(), g);
  fitness_ = EvaluateGenome(genome_);
}

void Individual::Swap(unsigned int index_1, unsigned int index_2) {
  auto temp_piece = genome_.at(index_1);
  genome_.at(index_1) = genome_.at(index_2);
  genome_.at(index_2) = temp_piece;
  fitness_ = EvaluateGenome(genome_);
}

bool operator<(const Individual &ind_1, const Individual &ind_2) {
  if (ind_1.fitness_ < ind_2.fitness_) {  // first compare by individual fitness
    return true;
  } else if (ind_1.fitness_ > ind_2.fitness_) {
    return false;
  } else if (ind_1.genome_ < ind_2.genome_) {  // on the off-chance that both individuals
    return true;                               // have the same fitness we compare their genome
  } else {
    return false;
  }
}

std::ostream &operator<<(std::ostream &out, Individual &ind) {
  auto pages = SplitGeneToPages(ind.genome_);
  out << "Fitness: " << std::to_string(ind.fitness_) << " Pages: " << std::to_string(pages.size());
  return out;
}
