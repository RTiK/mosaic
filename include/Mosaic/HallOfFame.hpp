#ifndef MOSAIC_HALLOFFAME_H
#define MOSAIC_HALLOFFAME_H

#include <set>
#include <vector>
#include <functional>
#include "Individual.hpp"

/**
 * Hall of Fame maintains a collection of the best individuals ever seen during evolution.
 * It keeps a fixed number of the best performing individuals, regardless of their age.
 */
class HallOfFame {
 private:
  std::set<Individual> hall_;
  size_t max_size_;
  std::function<void(const Individual&, size_t)> on_insert_callback_;

 public:
  explicit HallOfFame(size_t max_size = 10);

  void Update(const Individual& individual);

  void Update(const std::set<Individual>& population);

  std::vector<Individual> GetBest() const;

  Individual GetBestIndividual() const;

  size_t Size() const { return hall_.size(); }

  bool Empty() const { return hall_.empty(); }

  void Print() const;

  /**
   * Set a callback function that will be called whenever an individual is added to the hall.
   * The callback receives the individual and its rank (1-based, where 1 is the best).
   *
   * @param callback Function to call on insertion. Set to nullptr to disable.
   */
  void SetOnInsertCallback(std::function<void(const Individual&, size_t)> callback);

  friend std::ostream& operator<<(std::ostream& out, const HallOfFame& hof);
};

#endif //MOSAIC_HALLOFFAME_H