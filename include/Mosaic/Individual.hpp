#ifndef MOSAIC_INDIVIDUAL_H
#define MOSAIC_INDIVIDUAL_H

#include <vector>
#include <random>
#include "Page.hpp"
#include "piece/Piece.hpp"
#include <opencv2/opencv.hpp>

/**
 * Weights used when computing Individual fitness.
 * Pass a customized instance to Individual or IndividualGeneration functions
 * to tune how variance and missing icons are penalized.
 */
struct FitnessWeights {
    double variance_weight = 1.9;
    double missing_icons_weight = 0.4;
};

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
  int birth_generation_;
  FitnessWeights weights_;

  void Evaluate();

 public:
  Individual();

  Individual(const Individual &ind);

  Individual(std::vector<std::shared_ptr<Piece>> &genome, int birth_generation, FitnessWeights weights = {});

  Individual(const Individual &ind, std::mt19937 &g, int birth_generation);

  /**
   * Returns the fitness of the individual.
   */
  double GetFitness() const { return fitness_; };

  /**
   * Returns the fitness weights used by this individual.
   */
  FitnessWeights GetWeights() const { return weights_; };

  /**
   * Returns the generation when the individual was created.
   */
  int GetBirthGeneration() const { return birth_generation_; }

  /**
   * Returns the pages in the individual.
   */
  std::vector<Page> GetPages() { return pages_; }

  /**
   * Creates pages from the genome.
   */
  static std::vector<Page> SplitGenomeIntoPages(std::vector<std::shared_ptr<Piece>> &genome);

  /**
   * Swaps two pieces in the genome effectively creating a new individual, hence, current generation is required.
   */
  void Swap(unsigned int index_1, unsigned int index_2, int generation);

  /**
   * Compares two individuals by their fitness. This is required for keeping the population sorted.
   */
  bool operator<(const Individual &ind_1) const;

  /**
   * Prints the fitness parameters of pages in the individual.
   * See Print method for per-piece output.
   */
  friend std::ostream &operator<<(std::ostream &out, Individual &ind);

  /**
   * Returns the genome of the individual.
   */
  std::vector<std::shared_ptr<Piece>> GetGenome() { return genome_; };

  /**
   * Returns the size of the genome.
   */
  int Size() const { return genome_.size(); };

  /**
   * Prints the pieces in the genome.
   * See operator<< for output of pages.
   */
  void Print();

  /**
   * Displays all pages of the individual in a window.
   */
  void Show();
  
};

#endif //MOSAIC_INDIVIDUAL_H
