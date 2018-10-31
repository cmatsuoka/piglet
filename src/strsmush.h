#ifndef SRC_STRSMUSH_H_
#define SRC_STRSMUSH_H_

#include <cstdint>
#include <string>


class StringSmusher {
 public:
    static int amount(std::wstring const&, std::wstring const&, wchar_t, uint32_t);
    static std::wstring smush(std::wstring const&, std::wstring const&, int, wchar_t, uint32_t);
};

#endif  // SRC_STRSMUSH_H_
