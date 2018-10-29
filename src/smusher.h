#ifndef SRC_SMUSHER_H_
#define SRC_SMUSHER_H_

#include <cstdint>
#include <string>
#include "figfont.h"


class Smusher {
    uint32_t mode;
    bool full_width;
    bool right2left;
    FIGfont& font;
    Lines output;
 public:
    Smusher(FIGfont &f);
    Lines get();
    bool empty();
    Smusher& clear();
    Smusher& push(std::string);
    Smusher& push(wchar_t);
    Smusher& trim(int);
    int length();
};


#endif  // SRC_SMUSHER_H_
