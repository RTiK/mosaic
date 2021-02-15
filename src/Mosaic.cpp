//
// Created by Artem Khatchatourov on 30.01.21.
//

#include <random>
#include "Piece.h"
#include "PageTools.h"
#include "Evaluator.h"


struct Individual {
    std::vector<std::shared_ptr<Piece>> genome;
    double fitness = MAXFLOAT;
};

//typedef std::vector<std::shared_ptr<Piece>> Individual;

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


int NUM_OF_PAGE_BREAKS = 2;
int NUM_OF_PIECES = 40;
int POPULATION = 100;
int MAX_GENERATIONS = 500;//100;

std::random_device rd;
std::mt19937 g(rd());
std::uniform_int_distribution<> random_gene(0, NUM_OF_PIECES-1);

double evaluate_individual(Individual& individual) {
    double fitness = 0.0;
    for (const auto page : splitGeneToPages(individual.genome)) {
        fitness += evaluate_page(*page);
    }
    return fitness;
}

Individual createShuffledCopy(Individual &individual) {
    Individual newIndividual(individual);
    std::shuffle(newIndividual.genome.begin(), newIndividual.genome.end(), g);
    individual.fitness = evaluate_individual(newIndividual);
    return newIndividual;
}

void mutation_swap(Individual& individual) {
    int i1 = random_gene(g);
    int i2 = random_gene(g);
    std::shared_ptr<Piece> temp = individual.genome.at(i1);
    individual.genome.at(i1) = individual.genome.at(i2);
    individual.genome.at(i2) = temp;
    individual.fitness = evaluate_individual(individual);
}

double evaluate(Individual& individual) {
    auto pages = splitGeneToPages(individual.genome);
    double total_fitness = 0.0;
    for (const std::shared_ptr<PageEdge>& page_edge : pages) {
        total_fitness += evaluate_page(*page_edge);
    }
    return total_fitness;
}

bool compare_individuals(Individual& a, Individual& b) {
    bool is_larger = a.fitness > b.fitness;
    return is_larger;
}


int main() {
    //g.seed(0);

    Individual template_individual = generateIndividual(NUM_OF_PIECES, NUM_OF_PAGE_BREAKS);

    print_individual(template_individual);

    std::priority_queue<Individual, std::vector<Individual>, decltype(&compare_individuals)>
            population(compare_individuals);

    for (int i = 0; i < POPULATION; i++) {
        population.push(createShuffledCopy(template_individual));
    }

    for (int i = 0; i < MAX_GENERATIONS; i++) {
        std::cout << "generation " << i << std::endl;

        //auto temp_population(&population);
        std::priority_queue<Individual, std::vector<Individual>, decltype(&compare_individuals)>
                temp_population(compare_individuals);
        temp_population.swap(population);

        std::cout << temp_population.size() << std::endl;
        std::cout << population.size() << std::endl;

        // pass through elites
        for (int j = 0; j < POPULATION / 10; j++) {
            population.push(temp_population.top());
            temp_population.pop();
            // TODO allow some mutations
        }

        // mutate
        for (int j = 0; j < POPULATION / 2; j++) {
            auto temp_i = temp_population.top();
            temp_population.pop();
            mutation_swap(temp_i);
            population.push(temp_i);
        }

        // fill remaining
        for (int j = 0; j < POPULATION - population.size(); j++) {
            population.push(createShuffledCopy(template_individual));
        }


    }

    auto best = population.top();

    print_individual(best);

    for (std::shared_ptr<PageEdge> page : splitGeneToPages(best.genome)) {
        show_page(*page, std::to_string(evaluate_page(*page)));
    }

    /*
    auto i1_pages = splitGeneToPages(i1);

    double i1_fitness = 0.0;

    for (const std::shared_ptr<PageEdge>& page_edge : i1_pages) {
        double page_fitness = evaluate_page(*page_edge);
        show_page(*page_edge, std::to_string(page_fitness));
        i1_fitness += page_fitness;
    }
    */

    /*
    i1_pages = splitGeneToPages(i1);

    i1_fitness = 0.0;

    for (const std::shared_ptr<PageEdge>& page_edge : i1_pages) {
        double page_fitness = evaluate_page(*page_edge);
        show_page(*page_edge, std::to_string(page_fitness));
        i1_fitness += page_fitness;
    }
    */
    return 0;
}