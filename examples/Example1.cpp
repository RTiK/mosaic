//
// Created by Artem Khatchatourov on 30.01.21.
//

#include <random>
#include "Piece.h"
#include "LabPiece.h"
#include "Individual.h"
#include "FileLogger.h"


const int kNumOfPageBreaks = 2;
const int kNumOfPieces = 40;
const int kPopulation = 100;
const int kMaxGenerations = 500;
const std::string LOGFILE = "/Users/rt/Desktop/mosaic_gray.csv";

std::random_device rd;
std::mt19937 g(rd());
std::uniform_int_distribution<> random_ind_length(0, kNumOfPieces - 1);
std::uniform_int_distribution<> random_color_value(0, 256);


void PrintBest(std::set<Individual> &population, int best_n) {
  auto current = population.begin();
  while (current != population.end() && best_n >= 0) {
    std::cout << current->GetFitness() << std::endl;
    current++;
    best_n--;
  }
}

Individual GenerateIndividualGray(int length, int page_breaks) {
  std::vector<std::shared_ptr<Piece>> genome(length + page_breaks);
  for (int i = 0; i < length; i++) {
    genome[i] = std::make_shared<Piece>(i);
  }

  for (int i = length; i < length + page_breaks; i++) {
    genome[i] = kPageBreak;
  }
  return Individual(genome);
}

Individual GenerateIndividualGrayRandom(int length, int page_breaks) {
  std::vector<std::shared_ptr<Piece>> genome(length + page_breaks);
  for (int i = 0; i < length; i++) {
    genome[i] = std::make_shared<Piece>(random_color_value(g) / 256.0);
  }

  for (int i = length; i < length + page_breaks; i++) {
    genome[i] = kPageBreak;
  }
  return Individual(genome);
}

Individual GenerateIndividualRgbRandom(int length, int page_breaks) {
  std::vector<std::shared_ptr<Piece>> genome(length + page_breaks);
  for (int i = 0; i < length; i++) {
    genome[i] = std::make_shared<Piece>(random_color_value(g) / 256.0,
                                        random_color_value(g) / 256.0,
                                        random_color_value(g) / 256.0);
  }

  for (int i = length; i < length + page_breaks; i++) {
    genome[i] = kPageBreak;
  }
  return Individual(genome);
}

Individual GenerateIndividualLabRandom(int length, int page_breaks) {
  std::vector<std::shared_ptr<Piece>> genome(length + page_breaks);
  for (int i = 0; i < length; i++) {
    genome[i] = std::make_shared<LabPiece>(random_color_value(g) / 256.0,
                                           random_color_value(g) / 256.0,
                                           random_color_value(g) / 256.0);
  }

  for (int i = length; i < length + page_breaks; i++) {
    genome[i] = kPageBreak;
  }
  return Individual(genome);
}

void PrintIndividual(Individual &individual) {
  for (std::shared_ptr<Piece> &p : individual.GetGenome()) {
    if (p == kPageBreak) {
      std::cout << "page break ";
    } else {
      std::cout << p->GetColor();
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
    temp_mat.at<ColorT>(i, 0) = p->get()->GetColor();
    std::cout << p->get()->GetColor() << std::endl;
    i++;
  }

  cv::Mat page_mat = temp_mat.reshape(3, 6);  // make rows page constant

  cv::Mat out_mat;
  cv::resize(page_mat, out_mat, cv::Size(), 50, 50, cv::INTER_NEAREST);
  cv::imshow(window_title, out_mat);
  cv::waitKey(0);
}

int main() {
  //g.seed(0);
  FileLogger logger(LOGFILE);
  Individual template_individual = GenerateIndividualLabRandom(kNumOfPieces, kNumOfPageBreaks);
      //GenerateIndividualLabRandom(NUM_OF_PIECES, NUM_OF_PAGE_BREAKS);
      //GenerateIndividualRgbRandom(NUM_OF_PIECES, NUM_OF_PAGE_BREAKS);
      //GenerateIndividualGray(NUM_OF_PIECES, NUM_OF_PAGE_BREAKS);
  PrintIndividual(template_individual);

  std::set<Individual> population{};
  FillShuffle(population, template_individual, kPopulation);

  for (int i = 0; i < kMaxGenerations; i++) {
    std::cout << "generation " << i << std::endl;

    std::set<Individual> temp_population{};
    temp_population.swap(population);

    // pass through elites (first 10%)
    PassThroughElites(population, temp_population, 10);

    // mutate (another 60%)
    MutateBest(population, temp_population, 60);

    // fill remaining (~30%)
    FillShuffle(population, template_individual, 30);

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
  }

  PrintBest(population, 10);

  auto best = *population.begin();

  std::cout << best << std::endl;

  for (const auto page : best.GetPages()) {
    ShowPage(page, std::to_string(page.GetFitness()));
  }

  return 0;
}