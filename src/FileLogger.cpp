#include "Mosaic/FileLogger.hpp"

FileLogger::FileLogger() {
  OpenLogFile(GenerateLogFileName());
}

FileLogger::FileLogger(const std::string &file_name) {
  OpenLogFile(file_name);
}

FileLogger::FileLogger(const std::string &file_name, char delimiter) : delimiter_{delimiter} {
  OpenLogFile(file_name);
}

FileLogger::~FileLogger() {
  logfile_.close();
}

void FileLogger::Log(const std::set<Individual> &pop) {
  for (const auto &iter : pop) {
    logfile_ << iter.GetFitness() << delimiter_;
  }
  logfile_ << std::endl;
}

std::string FileLogger::GenerateLogFileName() {
  std::string file_prefix = "mosaic";
  std::string file_ending = "csv";
  std::time_t time_object = std::time(nullptr);
  char datetime_code[16];
  std::strftime(datetime_code, sizeof(datetime_code), "%Y%m%d-%H%M%S", std::localtime(&time_object));
  return file_prefix + "_" + datetime_code + "." + file_ending;
}

bool FileLogger::OpenLogFile(const std::string &file_name) {
  logfile_.open(file_name, std::ios::out | std::ios::trunc);
  if (logfile_.is_open()) {
    std::cout << "Log file " + file_name + " is open" << std::endl;
  } else {
    std::cerr << "Logging to file not initialized" << std::endl;
  }
  return logfile_.is_open();
}
