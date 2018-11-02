#ifndef SRC_CHARSMUSH_H_
#define SRC_CHARSMUSH_H_

#include <cstdint>


class CharSmusher {
 public:
    static wchar_t smush(const wchar_t, const wchar_t, const wchar_t, bool, uint32_t);
};

#endif  // SRC_CHARSMUSH_H_
