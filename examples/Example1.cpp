
//
// Created by Artem Khatchatourov on 30.01.21.
//

#include <random>
#include "LabPiece.h"
#include "Individual.h"
#include "FileLogger.h"
#include "IndividualGeneration.h"

const int kNumOfPageBreaks = 2;
const int kNumOfPieces = 40;
const int kPopulation = 200;
const int kMaxGenerations = 1000;

std::random_device rd;
std::mt19937 g(rd());
std::uniform_int_distribution<> random_ind_length(0, kNumOfPieces - 1);


void PrintBest(std::set<Individual> &population, int best_n) {
  auto current = population.begin();
  while (current != population.end() && best_n >= 0) {
    std::cout << current->GetFitness() << std::endl;
    current++;
    best_n--;
  }
}

void PrintIndividual(Individual &individual) {
  for (std::shared_ptr<Piece> &p : individual.GetGenome()) {
    if (p == kPageBreak) {
      std::cout << "page break ";
    } else {
      std::cout << p->GetInternalColor();
    }
  }
  std::cout << std::endl;
}

double MeanFitness(std::set<Individual>& population) {
  double total_fitness = 0.0;
  for (const auto& individual : population) {
    total_fitness += individual.GetFitness();
  }
  unsigned int num_of_elements = population.size();
  return total_fitness / num_of_elements;
}

void PassThroughElites(std::set<Individual> &new_pop, const std::set<Individual> &old_pop, unsigned int num) {
  auto old_pop_iter = old_pop.begin();
  while (old_pop_iter != old_pop.end() && num > 0) {
    new_pop.insert(*old_pop_iter);
    old_pop_iter++;
    num--;
  }
}

void MutateBest(std::set<Individual> &new_pop, const std::set<Individual> &old_pop, unsigned int num) {
  auto old_pop_iter = old_pop.begin();
  while (old_pop_iter != old_pop.end() && num > 0) {
    Individual ind = *old_pop_iter;
    ind.Swap(random_ind_length(g), random_ind_length(g));
    new_pop.insert(ind);
    old_pop_iter++;
    num--;
  }
}

void FillShuffle(std::set<Individual> &new_pop, const Individual &template_individual, unsigned int num) {
  for (int i = num; i > 0; i--) {
    new_pop.emplace(template_individual, g);  // call copy construct with shuffle arg
  }
}

void ShowPage(const Page& page, const std::string &window_title) {
  cv::Mat temp_mat(Page::max_pieces_, 1, CV_32FC3, ColorT(0, 1, 0));
  cv::MatIterator_<ColorT> mat_iter = temp_mat.begin<ColorT>();

  int i = 0;
  for (std::shared_ptr<Piece> *p = page.GetFirstPiece(); p <= page.GetLastPiece(); p++) {
    temp_mat.at<ColorT>(i, 0) = p->get()->GetRepresentationColor();
    std::cout << p->get()->GetRepresentationColor() << std::endl;
    i++;
  }

  cv::Mat page_mat = temp_mat.reshape(3, 6);  // make rows page constant

  cv::Mat out_mat;
  cv::resize(page_mat, out_mat, cv::Size(), 50, 50, cv::INTER_NEAREST);
  cv::imshow(window_title, out_mat);
  cv::waitKey(0);
}

void LogIndividual(Individual &individual, std::string file_name) {
  std::string path = "/Users/rt/Desktop/test/";
  std::string extension = ".csv";
  std::ofstream out_file = std::ofstream();
  out_file.open(path + file_name + extension, std::ios::out | std::ios::trunc);
  for (Page page : individual.GetPages()) {
    out_file << page.GetDistances() << " " << page.GetVariance() << std::endl;
    out_file << page;
    out_file << std::endl;
  }
  out_file.close();
}

int main() {
  g.seed(0);
  FileLogger logger;
  Individual template_individual = individual_generation::GenerateIndividualLabRandom(kNumOfPieces, kNumOfPageBreaks, g);

  PrintIndividual(template_individual);

  std::set<Individual> population{};
  FillShuffle(population, template_individual, kPopulation);

  for (int i = 0; i < kMaxGenerations; i++) {
    std::cout << "generation " << i << std::endl;

    std::set<Individual> temp_population{};
    temp_population.swap(population);

    // pass through elites (first 10%)
    PassThroughElites(population, temp_population, 10 * kPopulation / 100);

    // mutate (another 60%)
    MutateBest(population, temp_population, 60 * kPopulation / 100);

    // fill remaining (~30%)
    FillShuffle(population, template_individual, kPopulation - population.size());

    logger.Log(population);

    std::cout << "mean fitness: " << MeanFitness(population) << " top 10: ";
    auto current_ind = population.begin();
    unsigned int iter = 0;
    while (iter < 10) {
      std::cout << current_ind->GetFitness() << ", ";
      current_ind++;
      iter++;
    }
    std::cout << std::endl;

    auto best = *population.begin();
    LogIndividual(best, "generation_" + std::to_string(i));

  }

  PrintBest(population, 10);

  auto best = *population.begin();

  std::cout << best << std::endl;
  PrintIndividual(best);
  LogIndividual(best, "generation_" + std::to_string(kMaxGenerations));

  for (const auto page : best.GetPages()) {
    ShowPage(page, std::to_string(page.GetDistances()) + " " + std::to_string(page.GetVariance()));
  }

  return 0;
}