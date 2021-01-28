//
// Created by Artem Khatchatourov on 23.01.21.
//

#ifndef MOSAIC_PAGETOOLS_H
#define MOSAIC_PAGETOOLS_H

#include "Piece.h"

typedef std::pair<std::shared_ptr<Piece>*, std::shared_ptr<Piece>*> PageEdge;

static const int ROWS_PER_PAGE = 6;
static const int COLUMNS_PER_PAGE = 4;
static const int MAX_PIECES_ON_PAGE = ROWS_PER_PAGE * COLUMNS_PER_PAGE;

static std::shared_ptr<Piece> PAGE_BREAK;

std::vector<std::shared_ptr<PageEdge>> splitGeneToPages(std::vector<std::shared_ptr<Piece>>& gene);

#endif //MOSAIC_PAGETOOLS_H
