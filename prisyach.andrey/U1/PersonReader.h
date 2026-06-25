#ifndef PERSON_READER_H_INCLUDED
#define PERSON_READER_H_INCLUDED

#include "Person.h"
#include <vector>
#include <cstddef>
#include <cstdio>

struct ReaderStatistics {
  std::size_t successCount;
  std::size_t ignoredCount;
};

class PersonReader {
public:
  PersonReader();
  ReaderStatistics readFromFile(std::FILE* input, std::vector<Person>& people);

private:
  static const std::size_t BUFFER_SIZE = 4096;
  char buffer_[BUFFER_SIZE];
  bool tryParseLine(const char* line, Person& person, std::vector<std::size_t>& seenIds);
};

#endif
