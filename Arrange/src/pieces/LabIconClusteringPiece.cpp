//
// Created by Artem Khatchatourov on 30.12.21.
//

#include "LabIconClusteringPiece.h"

void LabIconClusteringPiece::Analyze(cv::Mat &color, cv::Mat &mask) {
  assert(color.type() == CV_32FC3);
  assert(mask.type() == CV_8UC1);

  cv::Mat lab;
  cv::cvtColor(color, lab, cv::COLOR_BGR2Lab);

  int num_of_pixels = cv::countNonZero(mask);
  cv::Mat pixels(1, num_of_pixels, lab.type());

  auto image_iterator = lab.begin<cv::Vec3f>();
  auto mask_iterator = mask.begin<ushort>();
  auto pixels_iterator = pixels.begin<cv::Vec3f>();

  for (int i = 0; i < lab.cols * lab.rows; i++) {
    cv::Vec3f pixel = *image_iterator++;
    if (*mask_iterator++ > 0) {
      (*pixels_iterator++) = pixel;
    }
  }

  cv::Mat best_labels;
  cv::TermCriteria criteria(cv::TermCriteria::COUNT, 100, 1);
  double compactness = cv::kmeans(pixels, 2, best_labels, criteria, 3, cv::KmeansFlags::KMEANS_RANDOM_CENTERS, centers_);
  // TODO run with different number of clusters and choose the setting with minimal compactness

  DetermineWeights(best_labels);

  assert(weights_.size() == centers_.size());
}

void LabIconClusteringPiece::DetermineWeights(cv::Mat &labels) {
  int zeros = labels.rows - cv::countNonZero(labels);
  cv::Mat ones = cv::Mat::ones(labels.size(), labels.type());
  while (zeros != 0) {
    weights_.push_back(zeros);
    labels = labels - ones;
    zeros = labels.rows - cv::countNonZero(labels);
  }
}

LabIconClusteringPiece::LabIconClusteringPiece(std::string path) : IconPiece(path) {
  cv::Mat color, mask;
  SplitColorChannelsAndAlpha(original_image_, color, mask);
  Analyze(color, mask);
}

LabIconClusteringPiece::LabIconClusteringPiece(cv::Mat image) : IconPiece(image) {
  cv::Mat color, mask;
  SplitColorChannelsAndAlpha(original_image_, color, mask);
  Analyze(color, mask);
}

double LabIconClusteringPiece::Distance(const Piece &other) const {
  return EuclideanDistance(this, (LabIconClusteringPiece*) &other);
}

double EuclideanDistance(const LabIconClusteringPiece *p_1, const LabIconClusteringPiece *p_2) {
  double distance = 0;
  for (int i = 0; i < p_1->centers_.size(); i++) {
    cv::Vec3f c_1 = p_1->centers_[i];
    double w_1 = p_1->weights_[i];
    for (int j = 0; j < p_2->centers_.size(); j++) {
      cv::Vec3f c_2 = p_2->centers_[j];
      double w_2 = p_2->weights_[j];

      cv::Vec3f sq;
      cv::pow(c_1 - c_2, 2, sq);
      double summed_diff = cv::sum(sq)[0];

      distance += cv::sqrt(summed_diff) * w_1 * w_2;
    }
  }
  return distance;
}

cv::Vec3f LabIconClusteringPiece::DominatingColor() const {
  cv::Vec3f dominating_color;
  double dominating_weight = 0.0;
  for (int i = 0; i < weights_.size(); i++) {
    if (dominating_weight < weights_[i]) {
      dominating_weight = weights_[i];
      dominating_color = centers_[i];
    }
  }
  return dominating_color;
}
