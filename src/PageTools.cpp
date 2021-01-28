//
// Created by Artem Khatchatourov on 23.01.21.
//

#include "PageTools.h"


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