//
// Created by Artem Khatchatourov on 15.02.21.
//

#include "Individual.h"

#include <utility>

Individual::Individual() = default;

Individual::Individual(const Individual &ind) {
  genome_ = std::vector(ind.genome_);
  Evaluate();
}

Individual::Individual(std::vector<std::shared_ptr<Piece>> &genome) {
  genome_ = std::vector(genome);
  Evaluate();
}


Individual::Individual(const Individual &ind, std::mt19937 &g) {
  genome_ = std::vector(ind.genome_);
  std::shuffle(genome_.begin(), genome_.end(), g);
  Evaluate();
}

void Individual::Swap(unsigned int index_1, unsigned int index_2) {
  auto temp_piece = genome_.at(index_1);
  genome_.at(index_1) = genome_.at(index_2);
  genome_.at(index_2) = temp_piece;
  Evaluate();
}

std::ostream &operator<<(std::ostream &out, Individual &ind) {
  auto pages = ind.GetPages();
  out << "Fitness: " << std::to_string(ind.fitness_) << " Pages: " << std::to_string(pages.size());
  return out;
}

std::vector<Page> Individual::SplitGenomeIntoPages(std::vector<std::shared_ptr<Piece>> &genome) {
  std::vector<Page> pages{};
  std::shared_ptr<Piece> *first_in_page = nullptr;
  std::shared_ptr<Piece> *last_in_page = nullptr;

  for (auto *p = &genome.front(); p <= &genome.back(); p++) {
    if (*p == kPageBreak) {  // careful this is just a null-check
      if (first_in_page != nullptr) {
        pages.push_back(Page(first_in_page, last_in_page));
        first_in_page = nullptr;
        last_in_page = nullptr;
      }
    } else {
      if (first_in_page != nullptr) {
        last_in_page = p;
      } else {
        first_in_page = p;
      }

      if (last_in_page - first_in_page == Page::max_pieces_ - 1) {
        pages.push_back(Page(first_in_page, last_in_page));
        first_in_page = nullptr;
        last_in_page = nullptr;
      }
    }
  }

  if (first_in_page != nullptr) {
    if (last_in_page != nullptr) {
      pages.push_back(Page(first_in_page, last_in_page));
    } else {
      pages.push_back(Page(first_in_page, first_in_page));
    }
  }

  return pages;
}

void Individual::Evaluate() {
  pages_ = SplitGenomeIntoPages(genome_);
  auto eval = [](double total_fitness, Page page) {
    return total_fitness + page.GetFitness();
  };
  fitness_ = std::accumulate(pages_.begin(), pages_.end(), 0.0, eval);
}

bool Individual::operator<(const Individual &other) const {
  if (fitness_ < other.GetFitness()) {  // first compare by fitness
    return true;
  } else if (fitness_ > other.GetFitness()) {
    return false;
  } else if (genome_ < other.genome_) {  // on the off-chance that both individuals
    return true;                         // have the same fitness we compare their genome
  } else {
    return false;
  }
}
