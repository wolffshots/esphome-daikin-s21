#include "utils.h"
#include <string.h>

bool uint8_starts_with_str(const std::vector<uint8_t>& vec, const char* str) {
    if (vec.size() < strlen(str)) {
        return false;
    }
  
    for (size_t i = 0; i < strlen(str); i++)
    {
      if(vec[i] == static_cast<uint8_t>(str[i])) {
        continue;
      } else {
        return false;
      }
    }
    
    return true;
  }