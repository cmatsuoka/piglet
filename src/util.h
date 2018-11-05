#ifndef SRC_UTIL_H_
#define SRC_UTIL_H_

#include <string>
#include <vector>


namespace util {

std::string error();
void trim_right(std::wstring&);
bool starts_with(std::string const&, std::string const&);
bool starts_with(std::wstring const&, std::wstring const&);
bool ends_with(std::string const&, std::string const&);
bool ends_with(std::wstring const&, std::wstring const&);
std::vector<std::wstring> split_whitespace(std::wstring const&);

}  // namespace util


#endif  // SRC_UTIL_H
