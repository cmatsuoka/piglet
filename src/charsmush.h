#ifndef SRC_CHARSMUSH_H_
#define SRC_CHARSMUSH_H_

#include <cstdint>


class CharSmusher {
 public:
    static char smush(const char, const char, const char, bool, uint32_t);
};

#endif  // SRC_CHARSMUSH_H_
