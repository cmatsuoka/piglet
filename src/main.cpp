#include <unistd.h>
#include <iostream>
#include "figfont.h"

void usage(char *);


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

    std::cout << msg << std::endl;
}

void usage(char *cmd)
{
    std::cout << ""
"Usage: " << cmd << " \n"
"Options:\n"
"    -h          display usage information and exit"
    << std::endl;
}

