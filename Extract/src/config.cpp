//
// Created by Artem Khatchatourov on 31.07.21.
//

#include <iostream>
#include <vector>
#include "config.h"

Config::Config(int argc, char* argv[]) {
  auto args = ToStrings(argc, argv);
  if (HasHelp(args)) {
    PrintHelp();
  } else {
    // the last arg must be a file path
    input_path_ = std::filesystem::path(args.back());

    // find output path if provided
    auto iter_output_path = std::find(args.begin(), args.end(), arg_output_);
    if (iter_output_path != args.end()) {
      output_path_ = std::filesystem::path(*++iter_output_path);
      output_path_ = canonical(output_path_);
    }

    // find number of icons on page if provided
    auto iter_n_icons_page = std::find(args.begin(), args.end(), arg_n_icons_page_);
    if (iter_n_icons_page != args.end()) {
      n_icons_page_ = std::stoi(*++iter_n_icons_page);
    }

    // find number of icons in dock if provided
    auto iter_n_icons_dock = std::find(args.begin(), args.end(), arg_n_icons_dock_);
    if (iter_n_icons_dock != args.end()) {
      n_icons_dock_ = std::stoi(*++iter_n_icons_dock);
    }
  }
}

bool Config::HasHelp(std::list<std::string> args) const {
  return std::count(args.begin(), args.end(), arg_help_) > 0;
}

std::list<std::string> Config::ToStrings(int argc, char* argv[]) {
  std::list<std::string> arg_vector = std::list<std::string>();
  for (int i = 0; i < argc; i++) {
    arg_vector.emplace_back(argv[i]);
  }
  return arg_vector;
}

void Config::PrintHelp() const {
  std::cout << "iOS icon extractor from a desktop screenshot" << std::endl
            << "Usage: extract\t" << arg_output_ << " output directory (default: current directory)" << std::endl
            << "              \t" << arg_n_icons_page_ << " number of icons on page (default: 24)" << std::endl
            << "              \t" << arg_n_icons_dock_ << " number of icons in dock (default: 4)" << std::endl
            << "              \tscreenshot path"
            << std::endl
            << "       extract\t" << arg_help_ << " print help" << std::endl;
}

std::ostream &operator<<(std::ostream &out, Config &config) {
  out << "input file: " << config.input_path_ << std::endl
      << "output folder: " << config.output_path_ << std::endl
      << "icons on page: " << config.n_icons_page_ << ", icons in dock: " << config.n_icons_dock_ << std::endl;
  return out;
}

std::string Config::GetOutputPath() const {
  if (!exists(output_path_)) {
    create_directories(output_path_);
  }
  return output_path_.string() + std::filesystem::path::preferred_separator;
}
