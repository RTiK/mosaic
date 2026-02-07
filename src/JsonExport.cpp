#include "Mosaic/JsonExport.hpp"
#include "Mosaic/PageEvaluation.hpp"
#include "Mosaic/piece/IconPiece.hpp"
#include <typeinfo>

namespace json_export {

std::string PieceTypeToString(PieceType type) {
  switch (type) {
    case COLOR_PIECE:
      return "ColorPiece";
    case LAB_ICON_PIECE:
      return "LabIconPiece";
    default:
      return "Unknown";
  }
}

json SerializeColor(const cv::Vec3f &color) {
  return json::array({color[0], color[1], color[2]});
}

json SerializeWeightedColor(const WeightedColor &wc) {
  return json{
    {"color", SerializeColor(wc.color)},
    {"weight", wc.weight}
  };
}

json SerializePieceNeighbors(const Page &page, unsigned int piece_index,
                              unsigned int genome_base_index) {
  json neighbors = json::array();

  unsigned char neighbor_flags = page_evaluation::CalculateNeighbors(piece_index, page.Size());

  std::shared_ptr<Piece> *pieces = page.GetFirstPiece();
  const std::shared_ptr<Piece> &current_piece = pieces[piece_index];

  // Helper lambda to add a neighbor to the JSON array
  auto add_neighbor = [&](const char* direction, int offset, unsigned char flag, double weight) {
    if (neighbor_flags & flag) {
      const std::shared_ptr<Piece> &neighbor = pieces[piece_index + offset];
      double distance = current_piece->Distance(*neighbor);
      double weighted_distance = distance * weight;

      neighbors.push_back(json{
        {"direction", direction},
        {"neighbor_genome_index", genome_base_index + piece_index + offset},
        {"neighbor_page_position", piece_index + offset},
        {"distance", distance},
        {"weight", weight},
        {"weighted_distance", weighted_distance}
      });
    }
  };

  // Add all 8 possible neighbors
  add_neighbor("N", -static_cast<int>(page_evaluation::kWidth), Neighbours::N, 1.0);
  add_neighbor("NE", -(static_cast<int>(page_evaluation::kWidth) - 1), Neighbours::NE, page_evaluation::kDiagonalWeight);
  add_neighbor("E", 1, Neighbours::E, 1.0);
  add_neighbor("SE", page_evaluation::kWidth + 1, Neighbours::SE, page_evaluation::kDiagonalWeight);
  add_neighbor("S", page_evaluation::kWidth, Neighbours::S, 1.0);
  add_neighbor("SW", page_evaluation::kWidth - 1, Neighbours::SW, page_evaluation::kDiagonalWeight);
  add_neighbor("W", -1, Neighbours::W, 1.0);
  add_neighbor("NW", -(static_cast<int>(page_evaluation::kWidth) + 1), Neighbours::NW, page_evaluation::kDiagonalWeight);

  return neighbors;
}

json SerializePiece(const std::shared_ptr<Piece> &piece, unsigned int genome_index,
                    unsigned int page_position, const json &neighbors) {
  // Calculate row and column from page position
  unsigned int row = page_position / page_evaluation::kWidth;
  unsigned int column = page_position % page_evaluation::kWidth;

  // Calculate average neighbor distance
  double avg_neighbor_distance = 0.0;
  if (!neighbors.empty()) {
    double total_weighted_distance = 0.0;
    double total_weight = 0.0;
    for (const auto &neighbor : neighbors) {
      total_weighted_distance += neighbor["weighted_distance"].get<double>();
      total_weight += neighbor["weight"].get<double>();
    }
    avg_neighbor_distance = total_weight > 0 ? total_weighted_distance / total_weight : 0.0;
  }

  // Serialize quantified colors
  json quantified_colors = json::array();
  for (const auto &wc : piece->GetQuantifiedColors()) {
    quantified_colors.push_back(SerializeWeightedColor(wc));
  }

  json piece_json = json{
    {"genome_index", genome_index},
    {"page_position", page_position},
    {"row", row},
    {"column", column},
    {"main_color", SerializeColor(piece->GetMainColor())},
    {"quantified_colors", quantified_colors},
    {"neighbors", neighbors},
    {"avg_neighbor_distance", avg_neighbor_distance}
  };

  // Add icon path if applicable
  std::string icon_path = GetIconPath(piece);
  if (!icon_path.empty()) {
    piece_json["icon_path"] = icon_path;
  }

  return piece_json;
}

json SerializePage(const Page &page, unsigned int page_index,
                   unsigned int genome_base_index) {
  // Calculate mean color for variance
  cv::Vec3f mean_color(0.0f, 0.0f, 0.0f);
  int num_pieces = 0;
  for (std::shared_ptr<Piece> *current = page.GetFirstPiece();
       current <= page.GetLastPiece(); current++) {
    mean_color += (*current)->GetMainColor();
    num_pieces++;
  }
  if (num_pieces > 0) {
    mean_color /= static_cast<float>(num_pieces);
  }

  // Serialize color distribution
  json color_distribution = json::array();
  for (const auto &wc : page.GetColorDistribution()) {
    color_distribution.push_back(SerializeWeightedColor(wc));
  }

  // Serialize all pieces on the page
  json pieces = json::array();
  unsigned int piece_index = 0;
  for (std::shared_ptr<Piece> *current = page.GetFirstPiece();
       current <= page.GetLastPiece(); current++, piece_index++) {
    json neighbors = SerializePieceNeighbors(page, piece_index, genome_base_index);
    unsigned int genome_index = genome_base_index + piece_index;
    pieces.push_back(SerializePiece(*current, genome_index, piece_index, neighbors));
  }

  return json{
    {"page_index", page_index},
    {"total_distances", page.GetDistances()},
    {"variance", page.GetVariance()},
    {"icons_missing", page.GetIconsMissing()},
    {"mean_color", SerializeColor(mean_color)},
    {"color_distribution", color_distribution},
    {"pieces", pieces}
  };
}

json SerializeIndividual(const Individual &individual, const ExportConfig &config) {
  // Build genome order array
  json genome_order = json::array();
  for (size_t i = 0; i < individual.Size(); i++) {
    genome_order.push_back(i);
  }

  // Serialize all pages
  json pages = json::array();
  auto page_list = const_cast<Individual&>(individual).GetPages();
  unsigned int genome_base_index = 0;

  for (size_t i = 0; i < page_list.size(); i++) {
    pages.push_back(SerializePage(page_list[i], i, genome_base_index));
    genome_base_index += page_list[i].Size();
  }

  return json{
    {"fitness", individual.GetFitness()},
    {"generation", individual.GetBirthGeneration()},
    {"piece_type", PieceTypeToString(config.piece_type)},
    {"config", {
      {"diagonal_weight", config.diagonal_weight},
      {"icons_missing_weight", config.icons_missing_weight},
      {"variance_weight", config.variance_weight},
      {"page_width", config.page_width},
      {"page_height", config.page_height}
    }},
    {"genome_order", genome_order},
    {"pages", pages}
  };
}

void ExportIndividualToNDJSON(const Individual &individual, const std::string &filepath, const ExportConfig &config) {
  json individual_json = SerializeIndividual(individual, config);

  std::ofstream file(filepath, std::ios::app);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file for writing: " + filepath);
  }

  file << individual_json.dump() << "\n";
  file.close();
}

std::string GetIconPath(const std::shared_ptr<Piece> &piece) {
  // Try to dynamic cast to IconPiece
  const IconPiece* icon_piece = dynamic_cast<const IconPiece*>(piece.get());
  if (icon_piece != nullptr) {
    return icon_piece->GetPath();
  }
  return "";
}

} // namespace json_export
