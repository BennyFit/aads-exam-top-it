#include "PersonReader.h"
#include "UnsignedParser.h"
#include "StringTrim.h"
#include <cstring>
#include <algorithm>

PersonReader::PersonReader() {
}

ReaderStatistics PersonReader::readFromFile(std::FILE* input, std::vector<Person>& people) {
  ReaderStatistics stats;
  stats.successCount = 0;
  stats.ignoredCount = 0;
  
  std::vector<std::size_t> seenIds;
  
  while (std::fgets(buffer_, BUFFER_SIZE, input) != nullptr) {
    Person person;
    
    if (tryParseLine(buffer_, person, seenIds)) {
      people.push_back(person);
      stats.successCount++;
    } else {
      stats.ignoredCount++;
    }
  }
  
  return stats;
}

bool PersonReader::tryParseLine(const char* line, Person& person, std::vector<std::size_t>& seenIds) {
  const char* end = line;
  while (*end != '\0' && *end != '\n') {
    ++end;
  }
  
  const char* current = skipWhitespace(line, end);
  
  if (current >= end) {
    return false;
  }
  
  const char* idNext = nullptr;
  if (!parseUnsignedSizeT(current, end, person.id, idNext)) {
    return false;
  }
  
  for (std::size_t seenId : seenIds) {
    if (seenId == person.id) {
      return false;
    }
  }
  
  const char* infoStart = skipWhitespace(idNext, end);
  
  if (infoStart >= end) {
    return false;
  }
  
  person.info.clear();
  person.info.append(infoStart, end);
  
  seenIds.push_back(person.id);
  
  return true;
}
