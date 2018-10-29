#ifndef SRC_WRAPPER_H_
#define SRC_WRAPPER_H_

#include <string>
#include <functional>
#include "smusher.h"


enum class Align : int {
    Left,
    Center,
    Right
};

class Wrapper {
    Smusher& sm;         // the FIGcharacter smusher
    std::string buffer;  // buffer to keep our input text
    bool has_space;      // whether we should add an extra space
 public:
    int width;           // terminal width
    Align align;         // text alignment

    Wrapper(Smusher&, int);
    Wrapper& clear();
    bool push(std::string const&);
    bool push(wchar_t);
    Wrapper& wrap_str(std::string const&, std::function<void(Lines)>);
    Wrapper& wrap_word(std::string const&, std::function<void(Lines)>);
    Lines get();
    int length();
    bool empty();
};

#endif  // SRC_WRAPPER_H_
