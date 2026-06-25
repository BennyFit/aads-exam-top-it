#include "StringTrim.h"

const char* skipWhitespace(const char* begin, const char* end) {
  while (begin < end && (*begin == ' ' || *begin == '\t')) {
    ++begin;
  }
  return begin;
}
