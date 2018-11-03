#include "figfont.h"
#include <string>
#include <iostream>
#include "figchar.h"
#include "file_io.h"
#include "util.h"


FIGfont::FIGfont() :
    version('a'),
    baseline(0),
    max_length(0),
    comment_lines(0),
    right_to_left(false),
    count(0),
    chars{},
    hardblank('$'),
    height(0),
    old_layout(0),
    layout(0)
{
}

FIGfont::FIGfont(std::string const& path) : FIGfont()
{
    load(path);
}

FIGchar FIGfont::get(const wchar_t ch)
{
    if (chars.find(ch) != chars.end()) {
        return chars[ch];
    }

    if (ch == '\t') {
        return chars[' '];
    }

    return chars['\0'];
}

FIGfont& FIGfont::load(std::string const& path)
{
    InputFile f(path);

    std::wstring line;
    f.read_line(line);
    parse_header(line);
    
    // Skip comment lines
    for (int i = 0; i < comment_lines; i++) {
        f.read_line(line);
    }

    // Define default 0-code character
    chars['\0'] = FIGchar(height);

    // Load required characters
    for (int i = 32; i < 127; i++) {
        FIGchar c(f, height);
        chars[static_cast<wchar_t>(i)] = c;
    }
    int extra[]{ 196, 215, 220, 228, 246, 252, 223 };
    for (int i : extra) {
        FIGchar c(f, height);
        chars[static_cast<wchar_t>(i)] = c;
    }

    // Load code-tagged characters 
    for (;;) {
        f.read_line(line);
        if (f.eof()) {
            break;
        }
        int code = stoi(line.substr(0, line.find(L" ")), 0, 0);
        FIGchar c(f, height);
        chars[static_cast<wchar_t>(code)] = c;
    }

    return *this;
}

FIGfont& FIGfont::parse_header(std::wstring const& line)
{
    if (!util::starts_with(line, L"flf2") && !util::starts_with(line, L"tlf2")) {
        throw std::runtime_error("unsupported font format");
    }

    auto parms = util::split_whitespace(line);
    if (parms.size() < 9 || parms[0].size() < 6) {
        throw std::runtime_error("unsupported font format");
    }

    version = parms[0][4];
    hardblank = parms[0][5];
    height = stoi(parms[1]);
    baseline = stoi(parms[2]);
    max_length = stoi(parms[3]);
    old_layout = stoi(parms[4]);
    comment_lines = stoi(parms[5]);
    right_to_left = parms[6] == L"1";
    layout = stoi(parms[7]);
    count = stoi(parms[8]);

    return *this;
}
