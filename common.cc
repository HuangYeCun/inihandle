#include "common.h"

#ifdef _WIN32
#include <io.h>
#include <direct.h>
#include <atlstr.h>
#include "Windows.h"
#elif __linux__
#include <unistd.h>
#include <sys/stat.h>
#endif  //_WIN32

#include <string.h>

#define DIR_BUF 256

Common::Common() {}

std::string Common::GetCurrentDir() {
  char dir_buf[DIR_BUF];
  memset(dir_buf, 0, sizeof(dir_buf));

#ifdef _WIN32
  //_getcwd(dir_buf, sizeof(dir_buf) - 1);

  HMODULE hocx = GetModuleHandle((COM_MODULE_NAME));
  TCHAR szPath[MAX_PATH];
  ::GetModuleFileName(hocx, szPath, MAX_PATH);

  std::string szpathStr = ConvertString2UTF8(szPath);
  int pos = szpathStr.find_last_of('\\', szpathStr.length());
  std::string currentPath = szpathStr.substr(0, pos);
  return currentPath;
#endif

#ifdef __linux__
  getcwd(dir_buf, sizeof(dir_buf) - 1);
  return dir_buf;
#endif
}

int Common::Access(const std::string& filename) {
#ifdef _WIN32
  return _access(filename.c_str(), 0);
#elif __linux__
  return access(filename.c_str(), 0);
#endif  //_WIN32
}

void Common::TrimStrHeadTail(std::string& str) {
  //去掉首尾空格
  if (!str.empty()) {
    str.erase(0, str.find_first_not_of(" "));
    str.erase(str.find_last_not_of(" ") + 1);
  }
}

void Common::TrimStrAll(std::string& str) {
  //去掉所有空格
  std::string::size_type pos = 0;
  if (!str.empty()) {
    while (str.npos != (pos = str.find(" ", pos))) {
      str.erase(pos, 1);
    }
  }
}
