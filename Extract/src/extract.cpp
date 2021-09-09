//
// Created by Artem Khatchatourov on 17.07.21.
//

#include <opencv2/opencv.hpp>
#include "IconCenters.h"
#include "config.h"

const int kSquircleRadius = 64;
const int kMaxIconsOnPage = 24;
const int kMaxIconsInDock = 4;

bool IsInSquircle(const float x, const float y) {
  // shift the integer coordinates by half of integer because the equation expects the coordinate lines to go between
  // pixels and not through them
  float x_shifted = x + 0.5f;
  float y_shifted = y + 0.5f;

  // iOS icons are a little translucent at the edges. as a result, some of the background might shine trough and distort
  // the colors. we keep this number <1 to cut off some of the edge of the icon
  float icon_border = 0.97f;

  return std::pow(std::abs(x_shifted / (float) kSquircleRadius), 5)
      + std::pow(std::abs(y_shifted / (float) kSquircleRadius), 5) <= icon_border;
}

cv::Mat ReadIcon(const cv::Mat &image, const std::pair<int, int> icon_center) {
  int icon_width = 2 * kSquircleRadius;
  int icon_height = 2 * kSquircleRadius;
  cv::Mat icon = cv::Mat::zeros(cv::Size(icon_width, icon_height), CV_16UC4);

  for (int i = -kSquircleRadius; i < kSquircleRadius; i++) {  // horizontal; x
    for (int j = -kSquircleRadius; j < kSquircleRadius; j++) {  // vertical; y
      if (IsInSquircle((float) i, (float) j)) {
        int pos_x = icon_center.first + i;
        int pos_y = icon_center.second + j;
        assert(pos_x >= 0 && pos_x < image.size[0]);
        assert(pos_y >= 0 && pos_y < image.size[1]);
        cv::Vec<ushort, 4> c = image.at<cv::Vec<ushort, 4>>(pos_x, pos_y);
        icon.at<cv::Vec<ushort, 4>>(kSquircleRadius + i, kSquircleRadius + j) = c;
      }
    }
  }

  return icon;
}

void ExtractAndSavePageIcons(cv::Mat& image, int num_of_icons, const std::string& destination_dir) {
  assert(num_of_icons <= kMaxIconsOnPage);

  auto icon_center_iter = icon_centers::kIphoneXrPage.begin();

  for (int i = 0; i < num_of_icons; i++) {
    cv::Mat icon = ReadIcon(image, *icon_center_iter++);
    std::string file_name = "icon_" + std::to_string(i) + ".png";
    cv::imwrite(destination_dir + file_name, icon);
  }
}

void ExtractAndSaveDockIcons(cv::Mat& image, int num_of_icons, const std::string& destination_dir) {
  assert(num_of_icons <= kMaxIconsInDock);

  auto icon_center_iter = icon_centers::kIphoneXrDock.begin();

  for (int i = 0; i < num_of_icons; i++) {
    cv::Mat icon = ReadIcon(image, *icon_center_iter++);
    std::string file_name = "dock_" + std::to_string(i) + ".png";
    cv::imwrite(destination_dir + file_name, icon);
  }
}

cv::Mat ReadScreenshot(const std::string& path) {
  cv::Mat img = cv::imread(path, cv::IMREAD_UNCHANGED);
  std::cout << "height: " << img.size[0] << " width: " << img.size[1] << std::endl;
  std::cout << "type: " << cv::typeToString(img.type()) << std::endl;
  return img;
}


int main(int argc, char* argv[]) {

  Config config = Config(argc, argv);

  std::cout << config << std::endl;

  cv::Mat img = ReadScreenshot(config.GetInputPath());

  ExtractAndSavePageIcons(img, config.GetNumIconsOnPage(), config.GetOutputPath());

  ExtractAndSaveDockIcons(img, config.GetNumIconsInDock(), config.GetOutputPath());

  return 0;
}