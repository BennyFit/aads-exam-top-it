#include "DataStore.h"
#include "StringTrim.h"
#include "UnsignedParser.h"
#include <algorithm>
#include <cstring>

namespace {
const std::size_t BUFFER_SIZE = 4096;

const char* trimTrailingWhitespace(const char* begin, const char* end) {
  while (end > begin && (*(end - 1) == ' ' || *(end - 1) == '\t')) {
    --end;
  }
  return end;
}

bool parsePersonLine(const char* line,
                     std::size_t lineLength,
                     std::size_t& id,
                     const char*& descriptionStart,
                     const char*& descriptionEnd) {
  const char* begin = line;
  const char* end = line + lineLength;
  const char* current = skipWhitespace(begin, end);
  if (current >= end) {
    return false;
  }
  const char* next = nullptr;
  if (!parseUnsignedSizeT(current, end, id, next)) {
    return false;
  }
  current = skipWhitespace(next, end);
  if (current >= end) {
    return false;
  }
  descriptionStart = current;
  descriptionEnd = trimTrailingWhitespace(current, end);
  if (descriptionStart >= descriptionEnd) {
    return false;
  }
  return true;
}

bool parseMeetingLine(const char* line,
                      std::size_t lineLength,
                      std::size_t& id1,
                      std::size_t& id2,
                      std::size_t& duration) {
  const char* begin = line;
  const char* end = line + lineLength;
  const char* current = skipWhitespace(begin, end);
  if (current >= end) {
    return false;
  }
  const char* next = nullptr;
  if (!parseUnsignedSizeT(current, end, id1, next)) {
    return false;
  }
  current = skipWhitespace(next, end);
  if (current >= end) {
    return false;
  }
  if (!parseUnsignedSizeT(current, end, id2, next)) {
    return false;
  }
  current = skipWhitespace(next, end);
  if (current >= end) {
    return false;
  }
  if (!parseUnsignedSizeT(current, end, duration, next)) {
    return false;
  }
  current = skipWhitespace(next, end);
  return current >= end;
}

void addOrDescribePerson(std::map<std::size_t, DataStore::PersonData>& persons,
                         std::size_t id,
                         const std::string& description) {
  auto it = persons.find(id);
  if (it == persons.end()) {
    DataStore::PersonData data;
    data.described = true;
    data.description = description;
    persons[id] = data;
  } else if (!it->second.described) {
    it->second.described = true;
    it->second.description = description;
  }
}

}  // namespace

DataStore::DataStore()
  : persons_(),
    meetings_() {
}

bool DataStore::hasPerson(std::size_t id) const {
  return persons_.find(id) != persons_.end();
}

bool DataStore::hasDescription(std::size_t id) const {
  auto it = persons_.find(id);
  return it != persons_.end() && it->second.described;
}

const std::string& DataStore::description(std::size_t id) const {
  return persons_.find(id)->second.description;
}

void DataStore::setDescription(std::size_t id, const std::string& description) {
  auto it = persons_.find(id);
  if (it == persons_.end()) {
    PersonData data;
    data.described = true;
    data.description = description;
    persons_[id] = data;
    return;
  }
  it->second.described = true;
  it->second.description = description;
}

void DataStore::addPersonIfMissing(std::size_t id) {
  if (persons_.find(id) == persons_.end()) {
    PersonData data;
    data.described = false;
    persons_[id] = data;
  }
}

void DataStore::removePerson(std::size_t id) {
  persons_.erase(id);
  meetings_.erase(id);
}

bool DataStore::loadPersons(std::FILE* input, std::string& error) {
  char buffer[BUFFER_SIZE];
  while (std::fgets(buffer, BUFFER_SIZE, input) != nullptr) {
    const char* line = buffer;
    std::size_t length = 0;
    while (buffer[length] != '\0' && buffer[length] != '\n') {
      ++length;
    }
    const char* descriptionBegin = nullptr;
    const char* descriptionEnd = nullptr;
    std::size_t id = 0;
    if (!parsePersonLine(line, length, id, descriptionBegin, descriptionEnd)) {
      if (skipWhitespace(line, line + length) >= line + length) {
        continue;
      }
      error = "Invalid person data";
      return false;
    }
    std::string description(descriptionBegin, descriptionEnd);
    addOrDescribePerson(persons_, id, description);
  }
  return true;
}

bool DataStore::loadMeetings(std::FILE* input, std::string& error) {
  char buffer[BUFFER_SIZE];
  while (std::fgets(buffer, BUFFER_SIZE, input) != nullptr) {
    const char* line = buffer;
    std::size_t length = 0;
    while (buffer[length] != '\0' && buffer[length] != '\n') {
      ++length;
    }
    if (skipWhitespace(line, line + length) >= line + length) {
      continue;
    }
    std::size_t id1 = 0;
    std::size_t id2 = 0;
    std::size_t duration = 0;
    if (!parseMeetingLine(line, length, id1, id2, duration)) {
      error = "Invalid meeting data";
      return false;
    }
    if (id1 == id2) {
      addPersonIfMissing(id1);
      continue;
    }
    addPersonIfMissing(id1);
    addPersonIfMissing(id2);
    meetings_[id1].push_back(Meeting{id2, duration});
    meetings_[id2].push_back(Meeting{id1, duration});
  }
  return true;
}

void DataStore::anonymousIds(std::vector<std::size_t>& ids) const {
  ids.clear();
  for (auto const& pair : persons_) {
    if (!pair.second.described) {
      ids.push_back(pair.first);
    }
  }
}

void DataStore::meetingPairs(std::size_t id, std::vector<Meeting>& out) const {
  out.clear();
  auto it = meetings_.find(id);
  if (it == meetings_.end()) {
    return;
  }
  out = it->second;
}

void DataStore::commonContacts(std::size_t id1,
                               std::size_t id2,
                               std::vector<std::size_t>& out) const {
  out.clear();
  auto it1 = meetings_.find(id1);
  auto it2 = meetings_.find(id2);
  if (it1 == meetings_.end() || it2 == meetings_.end()) {
    return;
  }
  std::vector<std::size_t> list1;
  std::vector<std::size_t> list2;
  for (auto const& meeting : it1->second) {
    list1.push_back(meeting.other);
  }
  for (auto const& meeting : it2->second) {
    list2.push_back(meeting.other);
  }
  std::sort(list1.begin(), list1.end());
  std::sort(list2.begin(), list2.end());
  std::vector<std::size_t> result;
  std::set_intersection(list1.begin(), list1.end(), list2.begin(), list2.end(), std::back_inserter(result));
  result.erase(std::unique(result.begin(), result.end()), result.end());
  out.swap(result);
}

bool DataStore::outputPersons(const std::string& filename, std::string& error) const {
  std::FILE* output = std::fopen(filename.c_str(), "w");
  if (output == nullptr) {
    error = "Cannot open output file";
    return false;
  }
  for (auto const& pair : persons_) {
    if (!pair.second.described) {
      continue;
    }
    std::fprintf(output, "%zu %s\n", pair.first, pair.second.description.c_str());
  }
  std::fclose(output);
  return true;
}

bool DataStore::mergeAnonymous(std::size_t anonymousId, std::size_t describedId) {
  auto anonIt = persons_.find(anonymousId);
  auto descIt = persons_.find(describedId);
  if (anonIt == persons_.end() || descIt == persons_.end()) {
    return false;
  }
  if (anonIt->second.described || !descIt->second.described) {
    return false;
  }
  auto anonMeetingsIt = meetings_.find(anonymousId);
  if (anonMeetingsIt != meetings_.end()) {
    for (auto const& meeting : anonMeetingsIt->second) {
      if (meeting.other == describedId) {
        auto it = meetings_.find(describedId);
        if (it != meetings_.end()) {
          std::vector<Meeting> filtered;
          filtered.reserve(it->second.size());
          for (auto const& entry : it->second) {
            if (entry.other != anonymousId) {
              filtered.push_back(entry);
            }
          }
          it->second.swap(filtered);
        }
        continue;
      }
      meetings_[describedId].push_back(Meeting{meeting.other, meeting.duration});
      auto otherIt = meetings_.find(meeting.other);
      if (otherIt != meetings_.end()) {
        for (auto& entry : otherIt->second) {
          if (entry.other == anonymousId) {
            entry.other = describedId;
          }
        }
      }
    }
  }
  removePerson(anonymousId);
  return true;
}

