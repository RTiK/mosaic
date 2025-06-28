#ifndef MOSAIC_HALLOFFAME_H
#define MOSAIC_HALLOFFAME_H

#include <set>
#include <vector>
#include "Individual.hpp"

/**
 * Hall of Fame maintains a collection of the best individuals ever seen during evolution.
 * It keeps a fixed number of the best performing individuals, regardless of their age.
 */
class HallOfFame {
 private:
  std::set<Individual> hall_;
  size_t max_size_;

 public:
  explicit HallOfFame(size_t max_size = 10);

  void Update(const Individual& individual);

  void Update(const std::set<Individual>& population);

  std::vector<Individual> GetBest() const;

  Individual GetBestIndividual() const;

  size_t Size() const { return hall_.size(); }

  bool Empty() const { return hall_.empty(); }

  void Print() const;

  friend std::ostream& operator<<(std::ostream& out, const HallOfFame& hof);
};

#endif //MOSAIC_HALLOFFAME_H