#ifndef MOSAIC_SRC_PAGE_H_
#define MOSAIC_SRC_PAGE_H_

#include "piece/Piece.hpp"
#include <memory>
#include <opencv2/opencv.hpp>

/**
 * This class represents a page on the iOS device. It holds pointers to the first and the last icons of the page.
 * Iterating between these pointers traverses the genome in the individual.
 */
class Page {
 private:
  std::shared_ptr<Piece> *first_piece_, *last_piece_;
  double distances_;
  double variance_;
  std::vector<WeightedColor> color_distribution_;
  int icons_missing_;

  void Evaluate();

 public:

  Page(const Page &page) : first_piece_{page.first_piece_}, last_piece_{page.last_piece_},
      distances_{page.distances_}, variance_{page.variance_}, color_distribution_{page.color_distribution_},
      icons_missing_{page.icons_missing_} {};

  Page(std::shared_ptr<Piece> *first_piece, std::shared_ptr<Piece> *last_piece);

  /**
   * Returns total distance between pieces on the page.
   */
  double GetDistances() const { return distances_; }

  /**
   * Returns page variance.
   */
  double GetVariance() const { return variance_; }

  /**
   * Returns the number of missing icons on the page.
   */
  int GetIconsMissing() const { return icons_missing_; }

  /**
   * Returns all colors on the page sorted by weight.
   */
  std::vector<WeightedColor> GetColorDistribution() const { return color_distribution_; }

  /**
   * Returns the number of pieces on the page.
   */
  unsigned int Size() const;

  /**
   * Returns a pointer to the first piece on the page.
   */
  std::shared_ptr<Piece> *GetFirstPiece() const { return first_piece_; };

  /**
   * Returns a pointer to the last piece on the page.
   */
  std::shared_ptr<Piece> *GetLastPiece() const { return last_piece_; };

  /**
   * Overloads the << operator to print page information.
   */
  friend std::ostream& operator<<(std::ostream& os, Page &page);

  /**
   * Creates a visual representation of the page for displaying using OpenCV.
   */
  cv::Mat Image(int side=50, cv::Vec3f default_color=cv::Vec3f(0.0, 0.0, 0.0)) const;

  /**
   * Displays the page image in a window.
   */
  void Show(std::string &window_title, int side=50, cv::Vec3f default_color=cv::Vec3f(0.0, 0.0, 0.0)) const;
};

#endif //MOSAIC_SRC_PAGE_H_
