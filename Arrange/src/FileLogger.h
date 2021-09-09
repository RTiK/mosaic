//
// Created by Artem Khatchatourov on 28.02.21.
//

#ifndef MOSAIC_SRC_FILELOGGER_H_
#define MOSAIC_SRC_FILELOGGER_H_

#include "fstream"
#include "Individual.h"
#include <ctime>

class FileLogger {
 private:
  std::ofstream logfile_;
  char delimiter_ = ';';

  bool OpenLogFile(const std::string &file_name);
  static std::string GenerateLogFileName();
 public:
  FileLogger();

  explicit FileLogger(const std::string &file_name);

  FileLogger(const std::string &file_name, char delimiter);

  ~FileLogger();

  void Log(const std::set<Individual> &pop);
};

#endif //MOSAIC_SRC_FILELOGGER_H_
