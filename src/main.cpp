#include <iostream>
#include <clocale>
#include <cctype>
#include <locale>
#include "wrapper.h"
#include "util.h"
#include "cxxopts.hpp"

constexpr int DefaultWidth = 80;
constexpr char DefaultFont[] = "standard.flf";
constexpr char DefaultDir[] = "/usr/share/figlet";


namespace {

bool ends_with(std::string const& s, std::string const& ending) {
    if (s.length() < ending.length()) {
        return false;
    }
    return s.compare(s.length() - ending.length(), ending.length(), ending) == 0;
}

std::string find_font(std::string const& dir, std::string const& name)
{
    std::string n(name);

    if (!ends_with(name, ".flf") && !ends_with(name, ".tlf")) {
        n += ".flf";
    }

    if (n.front() == File::separator()) {
        return n;
    }

    std::string path = dir + File::separator() + n;

    if (File::exists(path)) {
        return path;
    }

    return n;
}

void write_tokens(Wrapper& wr, std::wstring const& s, std::function<void(FIGline)> flush)
{
    for (auto v : util::split_whitespace(s)) {
        wr.wrap_str(v, flush);
    }
}

void write_line(Wrapper& wr, std::wstring const& s, std::function<void(FIGline)> flush)
{
    wr.clear();
    write_tokens(wr, s, flush);
    flush(wr.get());
}

void write_paragraph(Wrapper& wr, std::wstring const& s, std::function<void(FIGline)> flush)
{
    if (std::isspace(s.front())) {
        flush(wr.get());
        wr.clear();
    }
    write_tokens(wr, s, flush);
}


// From https://stackoverflow.com/questions/1664476/is-it-possible-to-use-a-unicode-argv
#define ARR_LEN(x) (sizeof(x)/sizeof(x[0]))

std::wstring to_wstring(const char *s)
{
    std::wstring res;

    if (s == nullptr || *s == 0) {
        return res;
    }

    typedef unsigned char byte;
    struct Level {
        byte Head, Data, Null;
        Level(byte h, byte d) {
            Head = h; // the head shifted to the right
            Data = d; // number of data bits
            Null = h << d; // encoded byte with zero data bits
        }
        bool encoded(byte b) { return b>>Data == Head; }
    };

    Level lev[] = {
        Level(2, 6),
        Level(6, 5),
        Level(14, 4),
        Level(30, 3),
        Level(62, 2),
        Level(126, 1)
    };

    wchar_t wc = 0;
    const char *p = s;

    while (*p != 0) {
        byte b = *p++;
        if (b>>7 == 0) {  // deal with ASCII
            wc = b;
            res.push_back(wc);
            continue;
        }  // ASCII

        bool found = false;
        for (int i = 1; i < ARR_LEN(lev); ++i) {
            if (lev[i].encoded(b)) {
                wc = b ^ lev[i].Null;            // remove the head
                wc <<= lev[0].Data * i;
                for (int j = i; j > 0; --j) {    // trailing bytes
                    if (*p == 0) {
                        return res;              // unexpected
                    }
                    b = *p++;
                    if (!lev[0].encoded(b)) {    // encoding corrupted
                        return res;
                    }
                    wchar_t tmp = b ^ lev[0].Null;
                    wc |= tmp << lev[0].Data*(j-1);
                }  // trailing bytes
                res.push_back(wc);
                found = true;
                break;
            }
        }

        if (!found) {
            return res;  // encoding incorrect
        }
    }
    return res;
}

}  // namespace


int main(int argc, char *argv[])
{
    std::setlocale(LC_CTYPE, "");

    int cols = DefaultWidth;
    std::string font_dir(DefaultDir);
    std::string font_name(DefaultFont);

    cxxopts::Options options(argv[0]);
    options.add_options()
        ("c,center", "center the output horizontally")
        ("d,dir", "set the default font directory", cxxopts::value<std::string>(font_dir))
        ("f,font", "specify the figfont to use", cxxopts::value<std::string>(font_name))
        ("h,help", "show usage information and exit")
        ("k,kern", "use kerning mode to display characters")
        ("l,left", "left-align the output")
        //("m,mode", "override the font layout mode", cxxopts::value<uint32_t>(num));
        ("o,overlap", "use character overlapping mode")
        ("p,paragraph", "ignore mid-paragraph line breaks")
        ("r,right", "right-align the output")
        ("S,smush", "use smushing mode to display characters")
        ("W,full-width", "display characters in full width")
        ("w,width", "set the output width", cxxopts::value<int>(cols));

    try {
        auto opt = options.parse(argc, argv);

        if (opt.count("help")) {
            std::cout << options.help() << std::endl;
            exit(EXIT_SUCCESS);
        }

        std::wstring msg;

        if (argc > 1) {
            // read message from command line
            msg += to_wstring(argv[1]);
            for (int i = 2; i < argc; i++) {
                msg = msg + L" " + to_wstring(argv[i]);
            }
        }

        auto font = FIGfont(find_font(font_dir, font_name));
        auto sm = Smusher(font);
    
        if (opt.count("o")) {
            sm.mode = 0;
        } else if (opt.count("k")) {
            sm.mode = SmushKern;
        } else if (opt.count("W")) {
            sm.full_width = true;
        }
    
        auto wr = Wrapper(sm, cols);
    
        if (opt.count("center")) {
           wr.align = Align::Center;
        } else if (opt.count("right")) {
           wr.align = Align::Right;
        } else if (opt.count("left")) {
           wr.align = Align::Left;
        }
    
        auto print_line = [](FIGline v) { std::wcout << v << std::flush; };

        if (msg.length() > 0) {
            // read message from command line parameters
            write_line(wr, msg, print_line);
        } else {
            // read message from stdin
            std::wstring line;
            std::getline(std::wcin, line);

            if (opt.count("paragraph")) {
                while (!std::wcin.eof()) {
                    msg += line;
                    write_paragraph(wr, line, print_line);
                    std::getline(std::wcin, line);
                }
                print_line(wr.get());
            } else {
                while (!std::wcin.eof()) {
                    msg += line;
                    write_line(wr, line, print_line);
                    std::getline(std::wcin, line);
                }
            }
        }

    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}
