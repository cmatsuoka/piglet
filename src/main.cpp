#include <unistd.h>
#include <iostream>
#include <locale>
#include "wrapper.h"
#include "cxxopts.hpp"

void print_output(FIGline);

constexpr int DefaultWidth = 80;
constexpr char DefaultFont[] = "standard.flf";
constexpr char DefaultDir[] = "/usr/share/figlet";


namespace {

std::string find_font(std::string const& dir, std::string const& name)
{
    std::string path = dir + File::separator() + name;

    // FIXME: add extension, check if file exists
    return path;
}


// From https://stackoverflow.com/questions/1664476/is-it-possible-to-use-a-unicode-argv
#define ARR_LEN(x) (sizeof(x)/sizeof(x[0]))

std::wstring to_wstring(const char *s)
{
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
    std::wstring result;

    while (*p != 0) {
        byte b = *p++;
        if (b>>7 == 0) { // deal with ASCII
            wc = b;
            result.push_back(wc);
            continue;
        } // ASCII

        bool found = false;
        for (int i = 1; i < ARR_LEN(lev); ++i) {
            if (lev[i].encoded(b)) {
                wc = b ^ lev[i].Null;            // remove the head
                wc <<= lev[0].Data * i;
                for (int j = i; j > 0; --j) {    // trailing bytes
                    if (*p == 0) return result;  // unexpected
                    b = *p++;
                    if (!lev[0].encoded(b))      // encoding corrupted
                        return result;
                    wchar_t tmp = b ^ lev[0].Null;
                    wc |= tmp << lev[0].Data*(j-1);
                } // trailing bytes
                result.push_back(wc);
                found = true;
                break;
            }
        }

        if (!found) {
            return result; // encoding incorrect
        }
    }
    return result;
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
        if (argc > 0) {
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
    
        auto wr = Wrapper(sm, 80);
    
        if (opt.count("center")) {
           wr.align = Align::Center;
        } else if (opt.count("right")) {
           wr.align = Align::Right;
        } else if (opt.count("left")) {
           wr.align = Align::Left;
        }
    
        wr.clear();
        wr.wrap_str(msg, print_output);
        print_output(wr.get());
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

void print_output(FIGline v)
{
    std::wcout << v << std::flush;
}

