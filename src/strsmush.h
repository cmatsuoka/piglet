#ifndef SRC_STRSMUSH_H_
#define SRC_STRSMUSH_H_

#include <cstdint>
#include <string>


class StringSmusher {
 public:
    static size_t amount(std::wstring const&, std::wstring const&, wchar_t, uint32_t);
    static std::wstring smush(std::wstring const&, std::wstring const&, size_t, wchar_t, uint32_t);
};

#endif  // SRC_STRSMUSH_H_
