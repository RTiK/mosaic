//
// Created by Artem Khatchatourov on 17.07.21.
//

#include <opencv2/opencv.hpp>
#include "IconIO.h"
#include "config.h"


int main(int argc, char* argv[]) {

  Config config = Config(argc, argv);
  std::cout << config << std::endl;  // debug

  cv::Mat screenshot = cv::imread(config.GetInputPath(), cv::IMREAD_UNCHANGED);
  std::cout << "Dimensions: " << screenshot.size << " Type: " << cv::typeToString(screenshot.type()) << std::endl;  // debug

  IconIO::ExtractAndSavePageIcons(screenshot, config.GetNumIconsOnPage(), config.GetOutputPath());
  IconIO::ExtractAndSaveDockIcons(screenshot, config.GetNumIconsInDock(), config.GetOutputPath());

  return 0;
}