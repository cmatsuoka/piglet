#include "wrapper.h"
#include <string>
#include <cctype>
#include <functional>
#include "util.h"


namespace {

FIGline add_pad(FIGline& v, int pad_size) {
    FIGline res;
    auto pad = std::wstring(pad_size, ' ');
    for (auto line : v) {
        res.push_back(pad + line);
    }
    return res;
}

}  // namespace


Wrapper::Wrapper(Smusher& smusher, int w) :
    sm(smusher),
    width(w),
    buffer{},
    align(Align::Left),
    has_space(true)
{
}

// Clear the output buffer.
Wrapper& Wrapper::clear()
{
    sm.clear();
    buffer.clear();
    has_space = true;
    return *this;
}

// Retrieve the output buffer lines.
FIGline Wrapper::get()
{
    if (length() > width) {
        sm.crop(width);
    }

    int w = width - length();
    auto v = sm.get();

    switch (align) {
    case Align::Center:
        return add_pad(v, w / 2);
    case Align::Right:
        return add_pad(v, w);
    case Align::Left:
    default:
        return v;
    }
}

// Get the length in sub-characters of the current output buffer.
int Wrapper::length()
{
    return sm.length();
}

// Verify whether the output buffer is empty.
bool Wrapper::empty()
{
    return sm.empty();
}

// Add a string to the output buffer.
bool Wrapper::push(std::wstring const &s)
{
    sm.push(s);
    if (sm.length() > width) {
        sm.clear();
        sm.push(buffer);
        return false;
    }
    buffer += s;
    return true;
}

// Add a character to the output buffer.
bool Wrapper::push(wchar_t ch)
{
    sm.push(ch);
    if (sm.length() > width) {
        sm.clear();
        sm.push(buffer);
        return false;
    }
    buffer += ch;
    return true;
}

// Tokenize string and add tokens to output buffer, wrapping if necessary.
Wrapper& Wrapper::wrap(std::wstring const& s, std::function<void(FIGline)> flush)
{
    for (auto v : util::split_whitespace(s)) {
        wrap_str(v, flush);
    }
    return *this;
}


// Add a string to the output buffer, wrapping it if necessary.
Wrapper& Wrapper::wrap_str(std::wstring const& ss, std::function<void(FIGline)> flush)
{
    std::wstring s(ss);

    util::trim_right(s);
    if (!has_space && !s.empty()) {
        push(' ');
    }

    has_space = s.empty();

    if (!push(s)) {
        if (!buffer.empty()) {
            flush(get());
            clear();
        }
        if (!push(s)) {
            wrap_word(s, flush);
        }
        has_space = false;
    }
    return *this;
}

// Add a word to the output buffer, breaking it if necessary.
Wrapper& Wrapper::wrap_word(std::wstring const& s, std::function<void(FIGline)> flush)
{
    for (auto c : s) {
        if (!push(c)) {
            if (!buffer.empty()) {
                flush(get());
                clear();
            }
            // don't wrap this character
            sm.push(c);
            buffer += c;
        }
    }

    return *this;
}

// Process an entire line, wrapping words if necessary.
Wrapper& Wrapper::wrap_line(std::wstring const& s, std::function<void(FIGline)> flush)
{
    clear();
    wrap(s, flush);
    flush(get());
    return *this;
}

// Process an entire paragraph, wrapping words if necessary.
Wrapper& Wrapper::wrap_paragraph(std::wstring const& s, std::function<void(FIGline)> flush)
{
    if (std::isspace(s.front())) {
        flush(get());
        clear();
    }
    wrap(s, flush);
    return *this;
}

