#include "wrapper.h"
#include <string>
#include <functional>


namespace {

    std::string trim(const std::string& str, const std::string& whitespace = " \t")
    {
        const auto start = str.find_first_not_of(whitespace);
        if (start == std::string::npos) {
            return "";  // no content
        }
        const auto end = str.find_last_not_of(whitespace);
        return str.substr(start, end - start + 1);
    }

    Lines add_pad(Lines& v, int pad_size) {
        Lines res;
        auto pad = std::string(pad_size, ' ');
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
Lines Wrapper::get()
{
    if (length() > width) {
        sm.trim(width);
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
bool Wrapper::push(std::string const &s)
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

// Add a string to the output buffer, wrapping it if necessary.
Wrapper& Wrapper::wrap_str(std::string const& ss, std::function<void(Lines)> flush)
{
    auto s = trim(ss);
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
Wrapper& Wrapper::wrap_word(std::string const& s, std::function<void(Lines)> flush)
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
