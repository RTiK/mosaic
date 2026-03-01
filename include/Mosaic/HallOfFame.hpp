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

  /**
   * Add the individual to the hall of fame if it is among the best. If the hall exceeds max_size, the worst individual is removed.
   */
  void Update(const Individual& individual);

  /**
   * Update the hall of fame with a new population. Only individuals better than the worst in the hall will be considered for insertion.
   */
  void Update(const std::set<Individual>& population);

  /**
   * Returns all individuals in the hall of fame, sorted by fitness.
   */
  std::vector<Individual> GetBest() const;

  /** 
   * Returns the best individual in the hall of fame.
   */
  Individual GetBestIndividual() const;

  /**
   * Returns the number of individuals in the hall of fame.
   */
  size_t Size() const { return hall_.size(); }

  /**
   * Checks if the hall of fame is empty.
   */
  bool Empty() const { return hall_.empty(); }

  /**
   * Prints all individuals in the hall of fame to standard output.
   */
  void Print() const;

  /**
   * Set a callback function that will be called whenever an individual is added to the hall.
   * The callback receives the individual and its rank (1-based, where 1 is the best).
   *
   * @param callback Function to call on insertion. Set to nullptr to disable.
   */
  void SetOnInsertCallback(std::function<void(const Individual&, size_t)> callback);

  /**
   * Overloads the << operator to print the hall of fame.
   */
  friend std::ostream& operator<<(std::ostream& out, const HallOfFame& hof);
};

#endif //MOSAIC_HALLOFFAME_H