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
  for (const auto& individual : population) {
    Update(individual);
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
    std::cout << "Rank " << rank++ << ": Fitness " << individual.GetFitness() << ", Birth " << individual.GetBirthGeneration()
              << ", Age " << individual.GetAge() << std::endl;
  }
}

std::ostream& operator<<(std::ostream& out, const HallOfFame& hof) {
  out << "Hall of Fame (" << hof.hall_.size() << "/" << hof.max_size_ << "):" << std::endl;
  int rank = 1;
  for (const auto& individual : hof.hall_) {
    out << "Rank " << rank++ << ": Fitness " << individual.GetFitness() << ", Birth " << individual.GetBirthGeneration()
        << ", Age " << individual.GetAge() << std::endl;
  }
  return out;
}