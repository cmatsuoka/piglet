#ifndef SRC_STRSMUSH_H_
#define SRC_STRSMUSH_H_

#include <cstdint>
#include <string>


class StringSmusher {
 public:
    static std::string smush(std::string const&, std::string const&, int, char, uint32_t);
};

#endif  // SRC_STRSMUSH_H_
