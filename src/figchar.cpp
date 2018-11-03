#include "figchar.h"
#include <iostream>
#include "file_io.h"
#include "util.h"


FIGchar::FIGchar() : lines()
{
}

FIGchar::FIGchar(int height) : lines(height)
{
}

FIGchar::FIGchar(FIGline ll) : lines()
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
    std::wstring line;
    for (int i = 0; i < height; i++) {
        line.clear();
        f.read_line(line);
        if (f.eof()) {
            for (auto it = lines.begin(); it != lines.end(); it++) {
                it->clear();
            }
            return;
        }

        util::trim_right(line);
        if (line.length() < 1) {
            throw std::runtime_error("invalid character width");
        }
        auto mark = line.back();
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

std::wostream& operator<<(std::wostream& os, FIGchar& c)
{
    auto v = c.get();
    return os << v;
}
