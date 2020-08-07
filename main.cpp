#include <iostream>
#include <vector>
#include <array>
#include <opencv2/opencv.hpp>


using namespace std;


const uint8_t POPULATION_SIZE = 10;


void getRandomColors(vector<cv::Vec<uint8_t, 3>> *colors, bool is_gray, uint16_t num_of_colors) {
    for (int i = 0; i < num_of_colors; i++) {
        if (is_gray) {
            long gray = random();
            colors->push_back(cv::Vec<uint8_t, 3>(gray, gray, gray));
        } else {
            colors->push_back(cv::Vec<uint8_t, 3>(random(), random(), random()));
        }
    }
}

void generatePopulation(vector<cv::Vec<uint8_t, 3>*> pop [], vector<cv::Vec<uint8_t, 3>> * colors, uint8_t num_of_page_breaks) {
    for (int i = 0; i < POPULATION_SIZE; i++) {
        vector<cv::Vec<uint8_t, 3>*> ind;
        for (auto & color : *colors) {
            ind.push_back(&color);
        }
        for (int j = 0; j < num_of_page_breaks; j++) {
            ind.push_back(nullptr);
        }
        //shuffle(ind.begin(), ind.end(), random());
        pop[i] = ind;
    }
}

void fillWithRandomIndividuals(vector<cv::Vec<uint8_t, 3>> *population, bool is_gray, uint16_t gene_length) {

    for (int i = 0; i < POPULATION_SIZE; i++) {
        vector<cv::Vec<uint8_t, 3>> gene;
        for (int j = 0; j < gene_length; j++) {
            if (is_gray) {
                int grayscale = random();
                gene.emplace_back(grayscale, grayscale, grayscale);
            } else {
                gene.emplace_back(random(), random(), random());
            }
        }

        population[i] = gene;
    }
}

void showIndividual(vector<cv::Vec<uint8_t, 3>> individual) {

    cv::Mat temp_mat = cv::Mat(individual);

    cv::Mat out_mat;

    cv::resize(temp_mat, out_mat, cv::Size(), 10, 10, cv::INTER_NEAREST);

    cv::imshow("asdf", out_mat);

    cv::waitKey(0);
}

int main() {

    const uint16_t gene_length = 28;
    const bool gray = true;

    // get colors/values
    vector<cv::Vec<uint8_t, 3>> colors;
    getRandomColors(&colors, gray, gene_length);

    // init population
    vector<cv::Vec<uint8_t, 3>*> population [POPULATION_SIZE];
    generatePopulation(population, &colors, 3);

    // init population
    //vector<cv::Vec<uint8_t, 3>> population [POPULATION_SIZE];
    //fillWithRandomIndividuals(population, gray, gene_length);

    // define fitness function

    // call EA

    // visualize results
    showIndividual(colors);

    return 0;
}


