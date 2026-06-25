#ifndef DATA_STORE_H_INCLUDED
#define DATA_STORE_H_INCLUDED

#include <cstddef>
#include <cstdio>
#include <map>
#include <string>
#include <vector>

struct Meeting {
  std::size_t other;
  std::size_t duration;
};

class DataStore {
public:
  struct PersonData {
    bool described;
    std::string description;
  };

  DataStore();
  bool loadPersons(std::FILE* input, std::string& error);
  bool loadMeetings(std::FILE* input, std::string& error);
  bool hasPerson(std::size_t id) const;
  bool hasDescription(std::size_t id) const;
  const std::string& description(std::size_t id) const;
  void setDescription(std::size_t id, const std::string& description);
  bool mergeAnonymous(std::size_t anonymousId, std::size_t describedId);
  void anonymousIds(std::vector<std::size_t>& ids) const;
  void meetingPairs(std::size_t id, std::vector<Meeting>& out) const;
  void commonContacts(std::size_t id1, std::size_t id2, std::vector<std::size_t>& out) const;
  bool outputPersons(const std::string& filename, std::string& error) const;

private:
  std::map<std::size_t, PersonData> persons_;
  std::map<std::size_t, std::vector<Meeting>> meetings_;

  void addPersonIfMissing(std::size_t id);
  void removePerson(std::size_t id);
};

#endif
