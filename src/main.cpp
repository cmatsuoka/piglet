#include <unistd.h>
#include <iostream>
#include <locale>
#include "wrapper.h"

void usage(char *);
void print_output(FIGline);
static std::wstring to_wstring(const char *);


int main(int argc, char *argv[])
{
    std::setlocale(LC_CTYPE, "");

    int c;
    while ((c = getopt(argc, argv, "h")) != -1) {
        switch (c) {
        case 'h':  // show usage information and exit
            usage(argv[0]);
            exit(EXIT_SUCCESS);
        default:
            exit(EXIT_FAILURE);
        }
    }

    std::wstring msg;
    if (optind < argc) {
        msg += to_wstring(argv[optind]);
        for (int i = optind + 1; i < argc; i++) {
            msg = msg + L" " + to_wstring(argv[i]);
        }
    }

    auto font = FIGfont("fonts/standard.flf");
    auto sm = Smusher(font);
    auto wr = Wrapper(sm, 80);

    wr.clear();
    wr.wrap_str(msg, print_output);
    print_output(wr.get());
}

void usage(char *cmd)
{
    std::cout << ""
"Usage: " << cmd << " \n"
"Options:\n"
"    -h          display usage information and exit"
    << std::endl;
}

void print_output(FIGline v)
{
    std::wcout << v << std::flush;
}


// From https://stackoverflow.com/questions/1664476/is-it-possible-to-use-a-unicode-argv

#define ARR_LEN(x) (sizeof(x)/sizeof(x[0]))

static std::wstring to_wstring(const char *s)
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
}   // wstring Convert

