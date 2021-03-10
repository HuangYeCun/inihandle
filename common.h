#ifndef COMMON_H
#define COMMON_H

#include <string>

class Common {
 public:
  Common();

  std::string GetCurrentDir();
  int Access(const std::string& filename);

  void TrimStrHeadTail(std::string& str);
  void TrimStrAll(std::string& str);
};

#endif  // COMMON_H
