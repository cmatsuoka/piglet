#include "smusher.h"
#include <algorithm>
#include <string>
#include "figfont.h"


namespace {

    void trim(Lines& lines, int width)
    {
    }

    void smush(Lines& lines, FIGchar& c, char hardblank, bool full_width, uint32_t mode)
    {
    }

}  // namespace


Smusher::Smusher(FIGfont& f) :
    font(f),
    mode(f.layout),
    full_width(f.old_layout == -1),
    right2left(false),
    output{}
{
    for (int i = 0; i < font.height; i++) {
        output.push_back("");
    }
}

// Get the contents of the output buffer.
Lines Smusher::get()
{
    Lines res = output;
    for (auto it = res.begin(); it != res.end(); it++) {
        std::replace((*it).begin(), (*it).end(), font.hardblank, ' ');
    }
    return res;
}

// Verify whether output buffer is empty.
bool Smusher::empty()
{
    return output.size() == 0 || output[0].empty();
}

// Clear the output buffer.
Smusher& Smusher::clear()
{
    for (auto it = output.begin(); it != output.end(); it++) {
        (*it).clear();
    }
    return *this;
}

// Add a string to the output buffer, applying the smushing rules specified
// in the font layout.
Smusher& Smusher::push(std::string s)
{
    for (auto it = s.begin(); it != s.end(); it++) {
        push(*it);
    }
    return *this;
}

// Add a character to the output buffer, applying the smushing rules specified
// in the font layout.
Smusher& Smusher::push(wchar_t c)
{
    auto fc = font.get(c);
    smush(output, fc, font.hardblank, full_width, mode);
}

// Obtain the size, in sub-characters, of any line of the output buffer.
int Smusher::length()
{
    return (output.size() == 0) ? 0 : output[0].length();
} 

// Limit the size, in sub-characters, of the output buffer. If the buffer is
// longer than the specified size, the rightmost sub-characters will be removed.
Smusher& Smusher::trim(int width)
{
    ::trim(output, width);
    return *this;
}