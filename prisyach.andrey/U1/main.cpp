#include "CommandLineOptions.h"
#include "PersonReader.h"
#include "PersonWriter.h"
#include "Person.h"
#include <vector>
#include <cstdio>
#include <cstdlib>

int main(int argc, char* argv[]) {
  CommandLineOptions options;
  if (!options.parse(argc, argv)) {
    std::fprintf(stderr, "Invalid command line arguments\n");
    return 1;
  }
  
  std::FILE* inputFile = nullptr;
  if (options.hasInputFile()) {
    inputFile = std::fopen(options.inputFileName().c_str(), "r");
    if (inputFile == nullptr) {
      std::fprintf(stderr, "Cannot open input file\n");
      return 2;
    }
  } else {
    inputFile = stdin;
  }
  
  std::FILE* outputFile = nullptr;
  if (options.hasOutputFile()) {
    outputFile = std::fopen(options.outputFileName().c_str(), "w");
    if (outputFile == nullptr) {
      if (options.hasInputFile()) {
        std::fclose(inputFile);
      }
      std::fprintf(stderr, "Cannot open output file\n");
      return 2;
    }
  } else {
    outputFile = stdout;
  }
  
  PersonReader reader;
  std::vector<Person> people;
  ReaderStatistics stats = reader.readFromFile(inputFile, people);
  
  PersonWriter writer;
  writer.writeToFile(outputFile, people);
  writer.writeStatistics(stderr, stats.successCount, stats.ignoredCount);
  
  if (options.hasInputFile()) {
    std::fclose(inputFile);
  }
  if (options.hasOutputFile()) {
    std::fclose(outputFile);
  }
  
  return 0;
}
