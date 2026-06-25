#include "CommandLineOptions.h"
#include <cstdio>
#include <cstdlib>

int main() {
  CommandLineOptions options;
    if (!options.parse(argc, argv)) {
      std::fprintf(stderr, "Invalid command line arguments\n");
      return 1;
    }
  return 0;
}