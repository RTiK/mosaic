#ifndef MOSAIC_JSON_EXPORT_HPP_
#define MOSAIC_JSON_EXPORT_HPP_

#include "Individual.hpp"
#include "Page.hpp"
#include "piece/Piece.hpp"
#include <nlohmann/json.hpp>
#include <string>

namespace json_export {

using json = nlohmann::json;

enum PieceType {
  COLOR_PIECE,
  LAB_PIECE,
  LAB_ICON_PIECE
};

/**
 * Converts PieceType enum to string representation.
 */
std::string PieceTypeToString(PieceType type);

/**
 * Serializes a color (cv::Vec3f) to a JSON array [r, g, b].
 */
json SerializeColor(const cv::Vec3f &color);

/**
 * Serializes a WeightedColor struct to JSON object with color array and weight.
 */
json SerializeWeightedColor(const WeightedColor &wc);

/**
 * Serializes neighbor information for a specific piece.
 *
 * @param page The page containing the piece
 * @param piece_index Position of the piece on the page (0-23)
 * @param genome_base_index Genome index of the first piece on this page
 * @return JSON array of neighbor objects with direction, indices, distances
 */
json SerializePieceNeighbors(const Page &page, unsigned int piece_index,
                              unsigned int genome_base_index);

/**
 * Serializes a single piece with all its properties and neighbor relationships.
 *
 * @param piece The piece to serialize
 * @param genome_index Index in the Individual's genome
 * @param page_position Position on the page grid (0-23)
 * @param neighbors JSON array of neighbor information
 * @return JSON object representing the piece
 */
json SerializePiece(const std::shared_ptr<Piece> &piece, unsigned int genome_index,
                    unsigned int page_position, const json &neighbors);

/**
 * Serializes a page with all its metrics and pieces.
 *
 * @param page The page to serialize
 * @param page_index Page number (0-based)
 * @param genome_base_index Genome index of the first piece on this page
 * @return JSON object representing the page
 */
json SerializePage(const Page &page, unsigned int page_index,
                   unsigned int genome_base_index);

/**
 * Serializes a complete Individual to JSON format.
 * Algorithm parameters (weights, grid dimensions, diagonal weight) are read directly
 * from the Individual and PageEvaluation constants.
 *
 * @param individual The individual to serialize
 * @param piece_type Type of pieces in this individual
 * @return JSON object representing the entire individual
 */
json SerializeIndividual(const Individual &individual, PieceType piece_type);

/**
 * Exports an Individual as a single line of NDJSON (newline-delimited JSON).
 * This format allows appending multiple individuals to the same file.
 *
 * @param individual The individual to export
 * @param filepath Path to the output file (will be created or appended to)
 * @param piece_type Type of pieces in this individual
 */
void ExportIndividualToNDJSON(const Individual &individual, const std::string &filepath, PieceType piece_type);

/**
 * Gets the icon path from a piece if it's an IconPiece, otherwise returns empty string.
 *
 * @param piece The piece to extract path from
 * @return Icon file path or empty string
 */
std::string GetIconPath(const std::shared_ptr<Piece> &piece);

} // namespace json_export

#endif // MOSAIC_JSON_EXPORT_HPP_
