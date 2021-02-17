//
// Created by Artem Khatchatourov on 15.02.21.
//

#include "Individual.h"


Individual::Individual() { }

Individual::Individual(Individual& individual) {
    this->genome = std::vector<std::shared_ptr<Piece>>(individual.genome);
    this->fitness = individual.fitness;
}

Individual::Individual(Individual& individual, std::mt19937 g) {
    this->genome = std::vector<std::shared_ptr<Piece>>(individual.genome);
    std::shuffle(this->genome.begin(), this->genome.end(), g);
    this->fitness = evaluate_genome(this->genome);
}

std::string Individual::to_string() {
    auto pages = splitGeneToPages(this->genome);
    return std::to_string(this->fitness) + "/" + std::to_string(pages.size());
}

void Individual::swap(unsigned int index1, unsigned int index2) {
    auto temp_piece = this->genome.at(index1);
    this->genome.at(index1) = this->genome.at(index2);
    this->genome.at(index2) = temp_piece;
    this->fitness = evaluate_genome(this->genome);
}
