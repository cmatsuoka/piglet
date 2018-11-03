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
    std::wstring buffer; // buffer to keep our input text
    bool has_space;      // whether we should add an extra space
 public:
    int width;           // terminal width
    Align align;         // text alignment

    Wrapper(Smusher&, int);
    Wrapper& clear();
    bool push(std::wstring const&);
    bool push(wchar_t);
    Wrapper& wrap_str(std::wstring const&, std::function<void(FIGline)>);
    Wrapper& wrap_word(std::wstring const&, std::function<void(FIGline)>);
    Wrapper& wrap(std::wstring const&, std::function<void(FIGline)>);
    Wrapper& wrap_line(std::wstring const&, std::function<void(FIGline)>);
    Wrapper& wrap_paragraph(std::wstring const&, std::function<void(FIGline)>);
    FIGline get();
    int length();
    bool empty();
};

#endif  // SRC_WRAPPER_H_
