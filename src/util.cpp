#include "util.h"
#include <string>
#include <cstring>
#include <cctype>
#include <sstream>
#include <iterator>
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

bool starts_with(std::wstring const& s, std::wstring prefix)
{
    return s.compare(0, prefix.length(), prefix) == 0;
}

std::vector<std::wstring> split_whitespace(std::wstring const& s)
{
    // FIXME: use C++17 ranges when available
    std::wistringstream ss(s);
    std::istream_iterator<std::wstring, wchar_t> begin(ss), end;
    std::vector<std::wstring> v(begin, end);
    return v;
}

}  // namespace util

