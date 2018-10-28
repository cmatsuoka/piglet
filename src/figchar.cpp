#include "figchar.h"
#include <iostream>
#include <algorithm>
#include "file_io.h"


namespace {

    void trim_right(std::string &s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) { return !std::isspace(ch); }).base(), s.end());
    }

}  // namespace


FIGchar::FIGchar() : lines()
{
}

FIGchar::FIGchar(int height) : lines()
{
    for (int i = 0; i < height; i++) {
        lines.push_back("");
    }
}

FIGchar::FIGchar(Lines ll) : lines()
{
    if (!ll.empty()) {
        int width = ll[0].length();
        for (auto l : ll) {
            if (l.length() != width) {
                throw std::runtime_error("invalid character width");
            }
            lines.push_back(l);
        }
    }
}

FIGchar::FIGchar(InputFile& f, int height) : lines()
{
    std::string line = "";
    for (int i = 0; i < height; i++) {
        line.clear();
        f.read_line(line);
        if (f.eof()) {
            // read rest of lines
            // ...
        }

        trim_right(line);
        if (line.length() < 1) {
            throw std::runtime_error("invalid character width");
        }
        char mark = line.back();
        line.pop_back();
        if (line.back() == mark) {
            line.pop_back();
        }
        lines.push_back(line);
    }

}

bool FIGchar::operator==(FIGchar const& c) const
{
    return lines == c.lines;
}

bool FIGchar::operator!=(FIGchar const& c) const
{
    return lines != c.lines;
}

std::ostream& operator<<(std::ostream& os, FIGchar& c)
{
    auto v = c.get();
    for (auto s : v) {
        os << s << '\n';
    }
    return os;
}
