#include "CommandLineOptions.h"

CommandLineOptions::CommandLineOptions()
  : inputFileName_(),
    outputFileName_(),
    hasInputFile_(false),
    hasOutputFile_(false),
    valid_(false) {
}

bool CommandLineOptions::parse(int argc, char* argv[]) {
  if (argc < 1 || argc > 3) {
    valid_ = false;
    return false;
  }
  
  for (int i = 1; i < argc; ++i) {
    const char* arg = argv[i];
    
    if (arg[0] == 'i' && arg[1] == 'n' && arg[2] == ':') {
      if (hasInputFile_) {
        valid_ = false;
        return false;
      }
      inputFileName_ = arg + 3;
      hasInputFile_ = true;
    }
    else if (arg[0] == 'o' && arg[1] == 'u' && arg[2] == 't' && arg[3] == ':') {
      if (hasOutputFile_) {
        valid_ = false;
        return false;
      }
      outputFileName_ = arg + 4;
      hasOutputFile_ = true;
    }
    else {
      valid_ = false;
      return false;
    }
  }
  
  valid_ = true;
  return true;
}

bool CommandLineOptions::isValid() const {
  return valid_;
}

bool CommandLineOptions::hasInputFile() const {
  return hasInputFile_;
}

bool CommandLineOptions::hasOutputFile() const {
  return hasOutputFile_;
}

const std::string& CommandLineOptions::inputFileName() const {
  return inputFileName_;
}

const std::string& CommandLineOptions::outputFileName() const {
  return outputFileName_;
}
