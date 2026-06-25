#include "PersonWriter.h"
#include <cstdio>

PersonWriter::PersonWriter() {
}

void PersonWriter::writeToFile(std::FILE* output, const std::vector<Person>& people) {
  for (const Person& person : people) {
    std::fprintf(output, "%zu %s\n", person.id, person.info.c_str());
  }
}

void PersonWriter::writeStatistics(std::FILE* errors, std::size_t successCount, std::size_t ignoredCount) {
  std::fprintf(errors, "%zu %zu\n", successCount, ignoredCount);
}
