#include "util.h"
#include <string>
#include <cstring>
#include <cctype>
#include <vector>
#include <fstream>
#include <algorithm>


namespace util {

std::string error()
{
    char *s = strerror(errno);
    return std::string(s);
}

void trim_right(std::wstring& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) { return !std::isspace(ch); }).base(), s.end());
}

}  // namespace util

