#ifndef COMMAND_LINE_OPTIONS_H_INCLUDED
#define COMMAND_LINE_OPTIONS_H_INCLUDED

#include <string>

class CommandLineOptions {
public:
  CommandLineOptions();
  bool parse(int argc, char* argv[]);
  bool isValid() const;
  bool hasInputFile() const;
  bool hasOutputFile() const;
  const std::string& inputFileName() const;
  const std::string& outputFileName() const;

private:
  std::string inputFileName_;
  std::string outputFileName_;
  bool hasInputFile_;
  bool hasOutputFile_;
  bool valid_;
};

#endif
