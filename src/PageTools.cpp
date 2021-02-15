//
// Created by Artem Khatchatourov on 23.01.21.
//

#include "PageTools.h"
#include <opencv2/opencv.hpp>


std::vector<std::shared_ptr<PageEdge>> splitGeneToPages(std::vector<std::shared_ptr<Piece>>& gene) {
    std::vector<std::shared_ptr<PageEdge>> pages{};
    std::shared_ptr<PageEdge> current_page_edge_ptr;

    for (auto* p = const_cast<std::shared_ptr<Piece>*>(&gene.front()); p <= const_cast<std::shared_ptr<Piece>*>(&gene.back()); p++) {
        if (*p == PAGE_BREAK) {
            if (current_page_edge_ptr) {
                current_page_edge_ptr.reset();
            }
        } else {
            if (current_page_edge_ptr) {
                current_page_edge_ptr->second = p;
            } else {
                current_page_edge_ptr = std::make_shared<PageEdge>(const_cast<std::shared_ptr<Piece>*>(p), const_cast<std::shared_ptr<Piece>*>(&gene.front()));
                current_page_edge_ptr->first = p;
                pages.emplace_back(current_page_edge_ptr);
            }

            long pieces_on_page = current_page_edge_ptr->second - current_page_edge_ptr->first + 1;
            if (pieces_on_page >= MAX_PIECES_ON_PAGE) {
                current_page_edge_ptr.reset();
            }
        }
    }

    return pages;
}

void show_page(PageEdge page_edge, std::string window_title) {

    cv::Mat temp_mat(MAX_PIECES_ON_PAGE, 1, CV_64FC3, color_t(0, 1, 0));

    cv::MatIterator_<color_t> mat_iter = temp_mat.begin<color_t>();

    int i = 0;
    for (std::shared_ptr<Piece>* p = page_edge.first; p <= page_edge.second; p++) {
        temp_mat.at<color_t>(i,0) = p->get()->getColor() / 100;
        i++;
    }

    cv::Mat page_mat = temp_mat.reshape(3, ROWS_PER_PAGE);

    cv::Mat out_mat;
    cv::resize(page_mat, out_mat, cv::Size(), 50, 50, cv::INTER_NEAREST);
    cv::imshow(window_title, out_mat);
    cv::waitKey(0);

}
