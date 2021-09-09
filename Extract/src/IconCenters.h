//
// Created by Artem Khatchatourov on 25.07.21.
//

#ifndef MOSAIC_EXTRACT_EXAMPLE_ICONCENTERS_H_
#define MOSAIC_EXTRACT_EXAMPLE_ICONCENTERS_H_

namespace icon_centers {
  // display size 828/1792
  const std::vector<std::pair<int, int>> kIphoneXrPage = {
      std::pair<int, int>(224, 128), std::pair<int, int>(224, 318), std::pair<int, int>(224, 510), std::pair<int, int>(224, 700),  // 1st row
      std::pair<int, int>(434, 128), std::pair<int, int>(434, 318), std::pair<int, int>(434, 510), std::pair<int, int>(434, 700),  // 2nd row
      std::pair<int, int>(644, 128), std::pair<int, int>(644, 318), std::pair<int, int>(644, 510), std::pair<int, int>(644, 700),  // 3rd row
      std::pair<int, int>(854, 128), std::pair<int, int>(854, 318), std::pair<int, int>(854, 510), std::pair<int, int>(854, 700),  // 4th row
      std::pair<int, int>(1064,128), std::pair<int, int>(1064,318), std::pair<int, int>(1064,510), std::pair<int, int>(1064,700),  // 5th row
      std::pair<int, int>(1274,128), std::pair<int, int>(1274,318), std::pair<int, int>(1274,510), std::pair<int, int>(1274,700)   // 6th row
  };

  const std::vector<std::pair<int, int>>& kIphone11Page = kIphoneXrPage;

  const std::vector<std::pair<int, int>> kIphoneXrDock = {
      std::pair<int, int>(1664,128), std::pair<int, int>(1664,318), std::pair<int, int>(1664,510), std::pair<int, int>(1664,700),  // dock
  };

  const std::vector<std::pair<int, int>>& kIphone11Dock = kIphoneXrDock;
}

#endif //MOSAIC_EXTRACT_EXAMPLE_ICONCENTERS_H_
