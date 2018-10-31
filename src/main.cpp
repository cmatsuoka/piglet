#include <unistd.h>
#include <iostream>
#include "wrapper.h"

void usage(char *);
void print_output(FIGline);


int main(int argc, char *argv[])
{
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

    std::string msg;
    if (optind < argc) {
        msg += argv[optind];
        for (int i = optind + 1; i < argc; i++) {
            msg = msg + " " + argv[i];
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
    std::cout << v << std::flush;
}
