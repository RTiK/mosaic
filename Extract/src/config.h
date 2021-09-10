//
// Created by Artem Khatchatourov on 30.07.21.
//

#ifndef MOSAIC_EXTRACT_EXAMPLE_CONFIG_H_
#define MOSAIC_EXTRACT_EXAMPLE_CONFIG_H_

#include <filesystem>
#include <list>

class Config {
 private:
  // argument names
  const std::string arg_help_ = "-h";
  const std::string arg_output_ = "-o";
  const std::string arg_n_icons_page_ = "-n";
  const std::string arg_n_icons_dock_ = "-d";
  // TODO ask for iphone model, alternatively deduce it from screenshot dimensions

  std::filesystem::path input_path_;  // mandatory

  // default values
  int n_icons_page_ = 24;
  int n_icons_dock_ = 0;
  std::filesystem::path output_path_ = std::filesystem::current_path();

  bool HasHelp(std::list<std::string> args) const;
  void PrintHelp() const;
  static std::list<std::string> ToStrings(int argc, char* argv[]);

 public:
  Config(int argc, char* argv[]);

  int GetNumIconsOnPage() const { return n_icons_page_; };

  int GetNumIconsInDock() const { return n_icons_dock_; };

  std::string GetInputPath() const { return input_path_.string(); };

  /**
   * Returns the destination directory, ensures it exists.
   * @return Path to output directory
   */
  std::string GetOutputPath() const;

  friend std::ostream &operator<<(std::ostream &out, Config &config);
};

#endif //MOSAIC_EXTRACT_EXAMPLE_CONFIG_H_
