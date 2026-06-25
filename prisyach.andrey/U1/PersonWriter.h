#ifndef PERSON_WRITER_H_INCLUDED
#define PERSON_WRITER_H_INCLUDED

#include "Person.h"
#include <vector>
#include <cstdio>

class PersonWriter {
public:
  PersonWriter();
  void writeToFile(std::FILE* output, const std::vector<Person>& people);
  void writeStatistics(std::FILE* errors, std::size_t successCount, std::size_t ignoredCount);
};

#endif
