#include "smusher.h"
#include <algorithm>
#include <string>
#include "figfont.h"
#include "strsmush.h"


namespace {

    int amount(FIGline& lines, FIGchar& c, wchar_t hardblank, uint32_t mode)
    {
        int amt = 9999;
        auto clines = c.get();
        for (int i = 0; i < lines.size(); i++) {
            amt = std::min(amt, StringSmusher::amount(lines[i], clines[i], hardblank, mode));
        }
        return amt;
    }

    void crop(FIGline& lines, int width)
    {
        for (auto it = lines.begin(); it != lines.end(); it++) {
            *it = (*it).substr(0, width);
        }
    }

    void smush(FIGline& lines, FIGchar& c, wchar_t hardblank, bool full_width, uint32_t mode)
    {
        auto amt = full_width ? 0 : amount(lines, c, hardblank, mode);
        auto clines = c.get();
        for (int i = 0; i < lines.size(); i++) {
            lines[i] = StringSmusher::smush(lines[i], clines[i], amt, hardblank, mode);
        }
    }

}  // namespace


Smusher::Smusher(FIGfont& f) :
    font(f),
    mode(f.layout),
    full_width(f.old_layout == -1),
    right2left(false),
    output(font.height)
{
}

// Get the contents of the output buffer.
FIGline Smusher::get()
{
    FIGline res = output;
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
Smusher& Smusher::push(std::wstring s)
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
    return *this;
}

// Obtain the size, in sub-characters, of any line of the output buffer.
int Smusher::length()
{
    return (output.size() == 0) ? 0 : output[0].length();
} 

// Limit the size, in sub-characters, of the output buffer. If the buffer is
// longer than the specified size, the rightmost sub-characters will be removed.
Smusher& Smusher::crop(int width)
{
    ::crop(output, width);
    return *this;
}
