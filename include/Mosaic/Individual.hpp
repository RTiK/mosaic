#ifndef MOSAIC_INDIVIDUAL_H
#define MOSAIC_INDIVIDUAL_H

#include <vector>
#include <random>
#include "Mosaic/Page.hpp"
#include "Mosaic/piece/Piece.hpp"


/**
 * This class represents a regular individual from the sense of genetic programming. However, along with the genome and
 * its fitness it also holds the pages with the pieces.
 * This class performs eager evaluation after every operation on the genome is performed. This is because the individual
 * is later used in a set and when iterating through this set we are banned from calling non-const methods. Otherwise
 * the fitness might change and the set must be resorted.
 */
class Individual {
 private:
  double fitness_;
  std::vector<Page> pages_;
  std::vector<std::shared_ptr<Piece>> genome_;

  void Evaluate();

 public:
  Individual();

  Individual(const Individual &ind);

  explicit Individual(std::vector<std::shared_ptr<Piece>> &genome);

  Individual(const Individual &ind, std::mt19937 &g);

  double GetFitness() const { return fitness_; };

  std::vector<Page> GetPages() { return pages_; }

  static std::vector<Page> SplitGenomeIntoPages(std::vector<std::shared_ptr<Piece>> &genome);

  void Swap(unsigned int index_1, unsigned int index_2);

  bool operator<(const Individual &ind_1) const;

  /**
   * Prints the fitness parameters of pages in the individual.
   * See Print method for per-piece output.
   */
  friend std::ostream &operator<<(std::ostream &out, Individual &ind);

  std::vector<std::shared_ptr<Piece>> GetGenome() { return genome_; };

  int Size() const { return genome_.size(); };

  /**
   * Prints the pieces in the genome.
   * See operator<< for output of pages.
   */
  void Print();

  void Show();
};

#endif //MOSAIC_INDIVIDUAL_H
