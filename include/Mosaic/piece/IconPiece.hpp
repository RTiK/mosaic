#ifndef MOSAIC_ARRANGE_SRC_ICONPIECE_H_
#define MOSAIC_ARRANGE_SRC_ICONPIECE_H_

#include <filesystem>
#include "Piece.hpp"


class IconPiece : public Piece {
 private:
  std::string icon_name_;
  std::filesystem::path icon_path_;

  virtual void Analyze(cv::Mat &color, cv::Mat &mask) {};

 protected:
  cv::Mat original_image_;  // BGRA

 public:
  IconPiece(std::filesystem::path path);
  IconPiece(cv::Mat image, std::string name="");
  IconPiece(const IconPiece &piece) : icon_path_{piece.icon_path_},
    original_image_{cv::imread(icon_path_, cv::IMREAD_UNCHANGED)} {};

  /**
   * Splits an image consisting of three color channels and an alpha channel (e.g. BGRA) into two separate matrices.
   * @param image Color + alpha image of type `CV_16UC4`
   * @param colors Output matrix of type `CV_32FC3` containing three color channels
   * @param alpha Output matrix of type `CV_8U1C` containing the alpha channel
   */
  static void SplitColorChannelsAndAlpha(const cv::Mat &image, cv::Mat &colors, cv::Mat &alpha);

  std::string GetPath() const { return icon_path_.string(); };

  cv::Mat Image(int width, int height) const override;

  static cv::MatND GetHistogram(cv::Mat &colors, int channel, cv::Mat mask, int bins, float range[]);

  std::string Print() const override;
};

#endif //MOSAIC_ARRANGE_SRC_ICONPIECE_H_
