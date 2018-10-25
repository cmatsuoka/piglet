#ifndef SRC_FIGFONT_H_
#define SRC_FIGFONT_H_

#include <map>
#include "figchar.h"

constexpr uint32_t SmushEqual     = 1 << 0;
constexpr uint32_t SmushUnderline = 1 << 1;
constexpr uint32_t SmushHierarchy = 1 << 2;
constexpr uint32_t SmushPair      = 1 << 3;
constexpr uint32_t SmushBigX      = 1 << 4;
constexpr uint32_t SmushHardblank = 1 << 5;
constexpr uint32_t SmushKern      = 1 << 6;
constexpr uint32_t SmushEnable    = 1 << 7;


class FIGfont {
    char version;
    int baseline;
    int max_length;
    int comment_lines;
    bool right_to_left;
    int count;
    std::map<char, FIGchar&> chars;
 public:
    char hardblank;
    int height;
    int old_layout;
    int layout;
};


#endif  // SRC_FIGFONT_H
