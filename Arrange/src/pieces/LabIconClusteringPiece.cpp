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
  auto mask_iterator = mask.begin<uchar>();
  auto pixels_iterator = pixels.begin<cv::Vec3f>();

  while (image_iterator != lab.end<cv::Vec3f>()) {
    cv::Vec3f pixel = *image_iterator;
    uchar m = *mask_iterator;
    if (m > 0) {
      *pixels_iterator = pixel;
      pixels_iterator++;
    }
    image_iterator++;
    mask_iterator++;
  }

  cv::TermCriteria criteria(cv::TermCriteria::COUNT, 100, 1);
  cv::Mat best_labels, labels;

  int max_clusters = std::min(5, pixels.cols);
  for (int i = 1; i <= max_clusters; i++) {
    std::vector<cv::Vec3f> centers;
    double compactness = cv::kmeans(pixels, i, labels, criteria, 3, cv::KmeansFlags::KMEANS_RANDOM_CENTERS, centers);
    auto sizes = ClusterSizes(labels);
    int lower_limit = pixels.cols * 0.05;
    if (*std::min_element(sizes.begin(), sizes.end()) > lower_limit) {
      weights_ = sizes;
      centers_ = centers;
    }
    for (int i = 0; i < centers_.size(); i++) {
      std::cout << "\t" << weights_[i] << ": " << centers_[i] << "," << std::endl;
    }
    //ShowClusters(labels, mask);

  }

  assert(weights_.size() == centers_.size());
}

std::vector<int> LabIconClusteringPiece::ClusterSizes(cv::Mat labels) {
  std::vector<int> sizes {};
  int zeros = labels.rows - cv::countNonZero(labels);

  while (zeros != 0) {
    sizes.push_back(zeros);
    labels-=1;
    zeros = labels.rows - cv::countNonZero(labels);
  }
  return sizes;
}

LabIconClusteringPiece::LabIconClusteringPiece(std::string path) : IconPiece(path) {
  cv::Mat color, mask;
  SplitColorChannelsAndAlpha(original_image_, color, mask);
  Analyze(color, mask);
  DetermineDominatingColor();
}

LabIconClusteringPiece::LabIconClusteringPiece(cv::Mat image) : IconPiece(image) {
  cv::Mat color, mask;
  SplitColorChannelsAndAlpha(original_image_, color, mask);
  Analyze(color, mask);
  DetermineDominatingColor();
}

double LabIconClusteringPiece::Distance(const Piece &other) const {
  return EuclideanDistance(this, (LabIconClusteringPiece*) &other);
}

double LabIconClusteringPiece::EuclideanDistance(const LabIconClusteringPiece *p_1, const LabIconClusteringPiece *p_2) {
  double distance = 0;
  double weights_total = 0;
  for (int i = 0; i < p_1->centers_.size(); i++) {
    cv::Vec3f c_1 = p_1->centers_[i];
    double w_1 = p_1->weights_[i];
    for (int j = 0; j < p_2->centers_.size(); j++) {
      cv::Vec3f c_2 = p_2->centers_[j];
      double w_2 = p_2->weights_[j];

      cv::Vec3f sq;
      cv::pow(c_1 - c_2, 2, sq);
      double summed_diff = cv::sum(sq)[0];

      distance += cv::sqrt(summed_diff) * (w_1 + w_2);
      weights_total += w_1 + w_2;
    }
  }
  return distance / weights_total;
}

void LabIconClusteringPiece::DetermineDominatingColor() {
  cv::Vec3f dominating_color;
  double dominating_weight = 0.0;
  for (int i = 0; i < weights_.size(); i++) {
    if (dominating_weight < weights_[i]) {
      dominating_weight = weights_[i];
      dominating_color = centers_[i];
    }
  }
  dominating_color_ = dominating_color;
}

void LabIconClusteringPiece::ShowClusters(cv::Mat icon_array, cv::Mat mask) {
  cv::Vec3b label_colors[] = {
      cv::Vec3b(0, 0, 255),
      cv::Vec3b(0, 255, 0),
      cv::Vec3b(0, 255, 255),
      cv::Vec3b(255, 0, 0),
      cv::Vec3b(255, 0, 255),
      cv::Vec3b(255, 255, 0)
  };

  cv::Mat icon(mask.rows, mask.cols, CV_8UC3);
  auto arr_iter = icon_array.begin<int>();
  auto icon_iter = icon.begin<cv::Vec3b>();
  auto mask_iter = mask.begin<uchar>();
  while (mask_iter != mask.end<uchar>()) {
    cv::Vec3b pixel(0, 0, 0);
    if (*mask_iter++ > 0) {
      pixel = label_colors[*arr_iter++];
    }
    *icon_iter++ = pixel;
  }
  cv::imshow("", icon);
  cv::waitKey();
}
