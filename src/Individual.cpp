//
// Created by Artem Khatchatourov on 15.02.21.
//

#include "Mosaic/Individual.hpp"
#include <utility>
#ifdef _WIN32
  #include <numeric>
#endif

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
  out << "Fitness: " << std::to_string(ind.fitness_) << " Pages: " << std::to_string(pages.size()) << std::endl;
  for (auto page : pages) {
    out << page;
  }
  out << std::endl;
  return out;
}

std::vector<Page> Individual::SplitGenomeIntoPages(std::vector<std::shared_ptr<Piece>> &genome) {
  // we don't create pages right away because they are evaluated in the constructor
  // instead we keep a reference to the first and the last elements on the page and construct the pages in the end
  std::vector<std::array<std::shared_ptr<Piece> *, 2>> page_edges {};
  std::array<std::shared_ptr<Piece> *, 2> *current_page_edge = nullptr;

  for (auto *p = &genome.front(); p <= &genome.back(); p++) {
    // careful, kPageBreak is currently not initialized, therefore, this is just a null-check
    if (*p == kPageBreak) {
      current_page_edge = nullptr;
    } else {
      if (current_page_edge == nullptr) {
        page_edges.emplace_back(std::array<std::shared_ptr<Piece> *, 2> {p, p});
        current_page_edge = &page_edges.back();
      } else {
        current_page_edge->back() = p;
        if (current_page_edge->back() - current_page_edge->front() == Page::max_pieces_ - 1) {
          current_page_edge = nullptr;
        }
      }
    }
  }

  std::vector<Page> pages{};
  auto page_initializer = [](std::array<std::shared_ptr<Piece> *, 2> page_edge) -> Page {
    return Page(page_edge.front(), page_edge.back());
  };
  std::transform(page_edges.begin(), page_edges.end(), std::back_inserter(pages), page_initializer);
  return pages;
}

void Individual::Evaluate() {
  pages_ = SplitGenomeIntoPages(genome_);
  double total_variance = 0.0;
  double total_icons_missing = 0.0;
  double total_distance = 0.0;
  for (Page &page : pages_) {
    total_distance += page.GetDistances();
    total_variance += page.GetVariance();
    total_icons_missing += page.GetIconsMissing();
  }

  double variance_normalized = total_variance / genome_.size();
  double missing_icons_penalty = total_icons_missing * variance_normalized * 0.2;
  fitness_ = total_distance + total_variance * 0.5 + missing_icons_penalty;
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

void Individual::Print() {
  std::string page_break = "PB";
  for (std::shared_ptr<Piece> &p : genome_) {
    std::cout << (p == kPageBreak ? page_break : (*p).Print()) << " " ;
  }
  std::cout << std::endl;
}

void Individual::Show() {
  cv::Mat page_images[pages_.size()];
  for (int i = 0; i < pages_.size(); i++) {
    page_images[i] = pages_[i].Image();
  }
  cv::Mat individual_image;
  cv::hconcat(&page_images[0], pages_.size(), individual_image);
  cv::imshow(std::to_string(GetFitness()), individual_image);
  cv::waitKey();
}
