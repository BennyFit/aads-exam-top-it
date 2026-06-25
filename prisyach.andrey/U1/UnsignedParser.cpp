#include "UnsignedParser.h"

bool parseUnsignedSizeT(const char* begin,
                        const char* end,
                        std::size_t& value,
                        const char*& next) {
  if (begin >= end) {
    return false;
  }
  
  if (*begin < '0' || *begin > '9') {
    return false;
  }
  
  std::size_t result = 0;
  const char* current = begin;
  
  const std::size_t MAX_DIV_10 = static_cast<std::size_t>(-1) / 10;
  const std::size_t MAX_MOD_10 = static_cast<std::size_t>(-1) % 10;
  
  while (current < end && *current >= '0' && *current <= '9') {
    int digit = *current - '0';
    
    if (result > MAX_DIV_10 || 
        (result == MAX_DIV_10 && digit > static_cast<int>(MAX_MOD_10))) {
      return false;
    }
    
    result = result * 10 + digit;
    ++current;
  }
  
  value = result;
  next = current;
  return true;
}
