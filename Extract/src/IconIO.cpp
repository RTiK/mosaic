//
// Created by Artem Khatchatourov on 10.09.21.
//

#include "IconIO.h"

bool IconIO::IsInSquircle(const float x, const float y) {
  // shift the integer coordinates by half of integer because the equation expects the coordinate lines to go between
  // pixels and not through them
  float x_shifted = x + 0.5f;
  float y_shifted = y + 0.5f;

  // iOS icons are a little translucent at the edges. as a result, some of the background might shine trough and distort
  // the colors. we keep this number <1 to cut off some of the edge of the icon
  float icon_border = 0.97f;

  return std::pow(std::abs(x_shifted / (float) squircle_radius_), 5)
      + std::pow(std::abs(y_shifted / (float) squircle_radius_), 5) <= icon_border;
}

cv::Mat IconIO::ReadIcon(const cv::Mat &image, const std::pair<int, int> icon_center) {
  int icon_width = 2 * squircle_radius_;
  int icon_height = 2 * squircle_radius_;
  cv::Mat icon = cv::Mat::zeros(cv::Size(icon_width, icon_height), CV_16UC4);

  for (int i = -squircle_radius_; i < squircle_radius_; i++) {  // horizontal; x
    for (int j = -squircle_radius_; j < squircle_radius_; j++) {  // vertical; y
      if (IsInSquircle((float) i, (float) j)) {
        int pos_x = icon_center.first + i;
        int pos_y = icon_center.second + j;
        assert(pos_x >= 0 && pos_x < image.size[0]);
        assert(pos_y >= 0 && pos_y < image.size[1]);
        cv::Vec<ushort, 4> c = image.at<cv::Vec<ushort, 4>>(pos_x, pos_y);
        icon.at<cv::Vec<ushort, 4>>(squircle_radius_ + i, squircle_radius_ + j) = c;
      }
    }
  }

  return icon;
}

void IconIO::ExtractAndSavePageIcons(cv::Mat &image, int num_of_icons, const std::string &destination_dir) {
  assert(num_of_icons <= max_icons_on_page_);

  auto icon_center_iter = icon_centers::kIphoneXrPage.begin();

  for (int i = 0; i < num_of_icons; i++) {
    cv::Mat icon = ReadIcon(image, *icon_center_iter++);
    std::string file_name = "icon_" + std::to_string(i) + ".png";
    cv::imwrite(destination_dir + file_name, icon);
  }
}

void IconIO::ExtractAndSaveDockIcons(cv::Mat &image, int num_of_icons, const std::string &destination_dir) {
  assert(num_of_icons <= max_icons_in_dock_);

  auto icon_center_iter = icon_centers::kIphoneXrDock[num_of_icons].begin();

  for (int i = 0; i < num_of_icons; i++) {
    cv::Mat icon = ReadIcon(image, *icon_center_iter++);
    std::string file_name = "dock_" + std::to_string(i) + ".png";
    cv::imwrite(destination_dir + file_name, icon);
  }
}
