#include "util.h"
#include <string>
#include <cstring>
#include <cctype>
#include <sstream>
#include <iterator>
#include <vector>
#include <fstream>
#include <algorithm>

namespace {

template<typename charT>
bool starts_with(std::basic_string<charT> const& s, std::basic_string<charT> const& prefix)
{
    if (s.length() < prefix.length()) {
        return false;
    }
    return s.compare(0, prefix.length(), prefix) == 0;
}

template<typename charT>
bool ends_with(std::basic_string<charT> const& s, std::basic_string<charT> const& ending) {
    if (s.length() < ending.length()) {
        return false;
    }
    return s.compare(s.length() - ending.length(), ending.length(), ending) == 0;
}

}  // namespace


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

bool starts_with(std::string const& s, std::string const& prefix)
{
    return ::starts_with<char>(s, prefix);
}

bool starts_with(std::wstring const& s, std::wstring const& prefix)
{
    return ::starts_with<wchar_t>(s, prefix);
}

bool ends_with(std::string const& s, std::string const& ending)
{
    return ::ends_with<char>(s, ending);
}

bool ends_with(std::wstring const& s, std::wstring const& ending)
{
    return ::ends_with<wchar_t>(s, ending);
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

