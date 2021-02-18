//
// Created by Artem Khatchatourov on 30.01.21.
//

#include <random>
#include "Piece.h"
#include "PageTools.h"
#include "Individual.h"


int NUM_OF_PAGE_BREAKS = 2;
int NUM_OF_PIECES = 40;
int POPULATION = 100;
int MAX_GENERATIONS = 1000;

std::random_device rd;
std::mt19937 g(rd());
std::uniform_int_distribution<> distribution(0, NUM_OF_PIECES-1);


bool compare_individuals(Individual& a, Individual& b) {
    bool is_larger = a.fitness < b.fitness;
    return is_larger;
}

void print_best(std::list<Individual>& population, int best_n) {
    auto iter = population.begin();
    for (int i = 0; i < best_n; i++) {
        std::cout << iter->fitness << std::endl;
        iter++;
    }
}

Individual generateIndividual(int length, int page_breaks) {
    Individual pieces;
    pieces.genome.reserve(length + page_breaks);
    for (int i = 0; i < length; i++) {
        pieces.genome.push_back(std::make_shared<Piece>(i));
    }

    for (int i = 0; i < page_breaks; i++) {
        pieces.genome.push_back(PAGE_BREAK);
    }
    return pieces;
}

void print_individual(Individual &individual) {
    for (std::shared_ptr<Piece>& p : individual.genome) {
        if (p == PAGE_BREAK) {
            std::cout << "page break ";
        } else {
            std::cout << p->getColor();
        }
    }
    std::cout << std::endl;
}


int main() {
    //g.seed(0);

    Individual template_individual = generateIndividual(NUM_OF_PIECES, NUM_OF_PAGE_BREAKS);

    print_individual(template_individual);

    std::list<Individual> population{};

    for (int i = 0; i < POPULATION; i++) {
        population.emplace_back(template_individual, g);
    }

    population.sort(compare_individuals);

    for (int i = 0; i < MAX_GENERATIONS; i++) {
        std::cout << "generation " << i << std::endl;

        std::list<Individual> temp_population{};

        temp_population.swap(population);

        std::cout << temp_population.size() << std::endl;
        std::cout << population.size() << std::endl;

        // pass through elites
        for (int j = 0; j < POPULATION / 10; j++) {
            population.emplace_back(temp_population.front());
            temp_population.pop_front();
            // TODO allow some mutations
        }

        // mutate
        for (int j = 0; j < POPULATION / 2; j++) {
            auto temp_i = temp_population.front();
            temp_population.pop_front();
            temp_i.swap(distribution(g), distribution(g));
            population.emplace_back(temp_i);
        }

        // fill remaining
        for (int j = 0; j < POPULATION - population.size(); j++) {
            population.emplace_back(template_individual, g);
        }

        population.sort(compare_individuals);
    }

    print_best(population, 10);

    auto best = population.front();

    print_individual(best);

    for (const std::shared_ptr<PageEdge>& page : splitGeneToPages(best.genome)) {
        show_page(*page, std::to_string(evaluate_page(*page)));
    }

    return 0;
}