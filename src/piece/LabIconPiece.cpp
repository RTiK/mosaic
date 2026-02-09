#include "Mosaic/piece/LabIconPiece.hpp"


LabIconPiece::LabIconPiece(cv::Mat image) : IconPiece(image)  {
  cv::Mat bgr, mask;
  SplitColorChannelsAndAlpha(original_image_, bgr, mask);
  Analyze(bgr, mask);
}

LabIconPiece::LabIconPiece(std::string path) : IconPiece(path) {
  cv::Mat bgr, mask;
  SplitColorChannelsAndAlpha(original_image_, bgr, mask);
  Analyze(bgr, mask);
}

void LabIconPiece::Analyze(cv::Mat &colors, cv::Mat &mask) {
  assert(colors.type() == CV_32FC3);
  assert(mask.type() == CV_8UC1);

  // Convert BGR (0-1 float) to Lab
  // For CV_32FC3 input in 0-1 range, cvtColor produces L [0..100], a* [-128..127], b* [-128..127]
  cv::Mat lab;
  cv::cvtColor(colors, lab, cv::COLOR_BGR2Lab);

  // Prepare data for clustering in Lab space
  std::vector<cv::Point3f> samples;
  for(int y = 0; y < lab.rows; y++) {
    for(int x = 0; x < lab.cols; x++) {
      if(mask.at<uchar>(y, x) > 0) {
        cv::Vec3f pixel = lab.at<cv::Vec3f>(y, x);
        samples.push_back(cv::Point3f(pixel[0], pixel[1], pixel[2]));
      }
    }
  }

  // Convert samples to Mat for k-means
  cv::Mat points(samples.size(), 3, CV_32F);
  for(size_t i = 0; i < samples.size(); i++) {
    points.at<float>(i, 0) = samples[i].x;
    points.at<float>(i, 1) = samples[i].y;
    points.at<float>(i, 2) = samples[i].z;
  }

  // Perform k-means clustering in Lab space
  cv::Mat labels, centers;
  cv::kmeans(points, kClusters, labels,
              cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 10, 1.0),
              3, cv::KMEANS_PP_CENTERS, centers);

  // Count pixels per cluster to determine weights
  std::vector<int> cluster_counts(kClusters, 0);
  for(int i = 0; i < labels.rows; i++) {
    cluster_counts[labels.at<int>(i)]++;
  }

  // Store Lab cluster centers and weights
  quantified_colors_.clear();
  for(int i = 0; i < kClusters; i++) {
    WeightedColor dc;
    dc.color = cv::Vec3f(centers.at<float>(i, 0), centers.at<float>(i, 1), centers.at<float>(i, 2));
    dc.weight = static_cast<float>(cluster_counts[i]) / labels.rows;
    quantified_colors_.push_back(dc);
  }
}

double LabIconPiece::Distance(const Piece &other) const {
  const LabIconPiece* other_piece = dynamic_cast<const LabIconPiece*>(&other);
  if (!other_piece) return std::numeric_limits<double>::max();

  return EuclideanDistance(this, other_piece);
}

double LabIconPiece::EuclideanDistance(const LabIconPiece* p_1, const LabIconPiece* p_2) {
  double distance = 0.0;

  // Compare all color pairs between the two pieces in Lab space
  for (size_t i = 0; i < p_1->quantified_colors_.size(); i++) {
    for (size_t j = 0; j < p_2->quantified_colors_.size(); j++) {
      cv::Vec3f color1 = p_1->quantified_colors_[i].color;
      cv::Vec3f color2 = p_2->quantified_colors_[j].color;
      float weight1 = p_1->quantified_colors_[i].weight;
      float weight2 = p_2->quantified_colors_[j].weight;

      // Weighted Euclidean distance between this color pair
      cv::Vec3f diff = color1 - color2;
      distance += cv::norm(diff) * weight1 * weight2;
    }
  }

  return distance;
}

cv::Vec3f LabIconPiece::GetMainColor() const {
  cv::Vec3f mean_color(0.0f, 0.0f, 0.0f);
  for (const auto& dc : quantified_colors_) {
    mean_color += dc.color * dc.weight;
  }
  return mean_color;
}

std::vector<WeightedColor> LabIconPiece::GetQuantifiedColors() const {
  return quantified_colors_;
}
