//
// Created by Artem Khatchatourov on 23.01.21.
//

#ifndef MOSAIC_PAGETOOLS_H
#define MOSAIC_PAGETOOLS_H

#include "Piece.h"

typedef std::pair<std::shared_ptr<Piece> *, std::shared_ptr<Piece> *> PageEdge;

static const int kRowsPerPage = 6;
static const int kColumnsPerPage = 4;
static const int kMaxPiecesOnPage = kRowsPerPage * kColumnsPerPage;

static std::shared_ptr<Piece> PAGE_BREAK;

std::vector<std::shared_ptr<PageEdge>> SplitGeneToPages(std::vector<std::shared_ptr<Piece>> &gene);

void ShowPage(PageEdge page_edge, const std::string &window_title);

#endif //MOSAIC_PAGETOOLS_H
