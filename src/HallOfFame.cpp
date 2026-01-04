#include "Mosaic/HallOfFame.hpp"
#include <iostream>

HallOfFame::HallOfFame(size_t max_size) : max_size_(max_size) {}

void HallOfFame::Update(const Individual& individual) {
  hall_.insert(individual);
  
  // If we exceed max_size, remove the worst individual
  if (hall_.size() > max_size_) {
    auto worst = hall_.rbegin(); // Last element (worst fitness)
    hall_.erase(std::prev(worst.base()));
  }
}

void HallOfFame::Update(const std::set<Individual>& population) {
  // If hall is not full, just add best individuals until full
  if (hall_.size() < max_size_) {
    auto it = population.begin();
    size_t to_add = max_size_ - hall_.size();
    for (size_t i = 0; i < to_add && it != population.end(); ++i, ++it) {
      hall_.insert(*it);
    }
    return;
  }

  // Hall is full, get the worst fitness threshold
  double worst_fitness = std::prev(hall_.end())->GetFitness();

  // Only insert individuals better than the worst in hall
  for (const auto& individual : population) {
    if (individual.GetFitness() < worst_fitness) {  // Lower is better
      hall_.insert(individual);
      // Remove the new worst
      hall_.erase(std::prev(hall_.end()));
      // Update threshold
      worst_fitness = std::prev(hall_.end())->GetFitness();
    } else {
      // Since population is sorted, all remaining are worse
      break;
    }
  }
}

std::vector<Individual> HallOfFame::GetBest() const {
  return std::vector<Individual>(hall_.begin(), hall_.end());
}

Individual HallOfFame::GetBestIndividual() const {
  if (hall_.empty()) {
    throw std::runtime_error("Hall of Fame is empty");
  }
  return *hall_.begin(); // First element (best fitness)
}

void HallOfFame::Print() const {
  std::cout << "Hall of Fame (" << hall_.size() << "/" << max_size_ << "):" << std::endl;
  int rank = 1;
  for (const auto& individual : hall_) {
    std::cout << "Rank " << rank++ << ": Fitness " << individual.GetFitness() << ", Birth " << individual.GetBirthGeneration() << std::endl;
  }
}

std::ostream& operator<<(std::ostream& out, const HallOfFame& hof) {
  out << "Hall of Fame (" << hof.hall_.size() << "/" << hof.max_size_ << "):" << std::endl;
  int rank = 1;
  for (const auto& individual : hof.hall_) {
    out << "Rank " << rank++ << ": Fitness " << individual.GetFitness() << ", Birth " << individual.GetBirthGeneration() << std::endl;
  }
  return out;
}