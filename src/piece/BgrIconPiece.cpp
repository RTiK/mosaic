#include "Mosaic/piece/BgrIconPiece.hpp"


BgrIconPiece::BgrIconPiece(cv::Mat image) : IconPiece(image)  {
  cv::Mat bgr, mask;
  SplitColorChannelsAndAlpha(original_image_, bgr, mask);
  Analyze(bgr, mask);
}

BgrIconPiece::BgrIconPiece(std::string path) : IconPiece(path) {
  cv::Mat bgr, mask;
  SplitColorChannelsAndAlpha(original_image_, bgr, mask);
  Analyze(bgr, mask);
}

void BgrIconPiece::Analyze(cv::Mat &colors, cv::Mat &mask) {
  assert(colors.type() == CV_32FC3);
  assert(mask.type() == CV_8UC1);

  // Prepare data for clustering
  std::vector<cv::Point3f> samples;
  for(int y = 0; y < colors.rows; y++) {
    for(int x = 0; x < colors.cols; x++) {
      if(mask.at<uchar>(y, x) > 0) {
        cv::Vec3f pixel = colors.at<cv::Vec3f>(y, x);
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

  // Perform k-means clustering
  cv::Mat labels, centers;
  cv::kmeans(points, kClusters, labels,
              cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 10, 1.0),
              3, cv::KMEANS_PP_CENTERS, centers);

  // Count pixels per cluster to determine weights
  std::vector<int> cluster_counts(kClusters, 0);
  for(int i = 0; i < labels.rows; i++) {
    cluster_counts[labels.at<int>(i)]++;
  }

  // Store results
  dominant_colors_.clear();
  for(int i = 0; i < kClusters; i++) {
    DominantColor dc;
    dc.color = cv::Vec3f(centers.at<float>(i, 0), centers.at<float>(i, 1), centers.at<float>(i, 2));
    dc.weight = static_cast<float>(cluster_counts[i]) / labels.rows;
    dominant_colors_.push_back(dc);
  }

  // Sort by weight (most dominant first)
  std::sort(dominant_colors_.begin(), dominant_colors_.end(),[](const DominantColor& a, const DominantColor& b) {
    return a.weight > b.weight;
  });

  std::cout << "Dominant colors " << ":\n";
  for (const auto& dc : dominant_colors_) {
    std::cout << "Color: " << dc.color * 65535 << ", Weight: " << dc.weight << "\n";
  } 
}

double BgrIconPiece::Distance(const Piece &other) const {
  const BgrIconPiece* other_piece = dynamic_cast<const BgrIconPiece*>(&other);
  if (!other_piece) return std::numeric_limits<double>::max();
  
  return EuclideanDistance(this, other_piece);
}

double BgrIconPiece::EuclideanDistance(const BgrIconPiece* p_1, const BgrIconPiece* p_2) {
  double distance = 0.0;
  for (size_t i = 0; i < p_1->dominant_colors_.size(); i++) {
    cv::Vec3f color1 = p_1->dominant_colors_[i].color;
    cv::Vec3f color2 = p_2->dominant_colors_[i].color;
    float weight1 = p_1->dominant_colors_[i].weight;
    float weight2 = p_2->dominant_colors_[i].weight;
    
    // Weighted Euclidean distance
    cv::Vec3f diff = color1 - color2;
    distance += cv::norm(diff) * (weight1 + weight2) / 2.0;
  }
  return distance;
}

cv::Vec3f BgrIconPiece::DominatingColor() const {
  return dominant_colors_[0].color;  // Return the most dominant color
}