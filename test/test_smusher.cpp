#include "doctest.h"
#include "../src/smusher.cpp"

TEST_SUITE("smusher") {
    TEST_CASE("smusher::amount") {
        Lines output;
        FIGchar fc;

        output = {"", "", "", ""};
        fc = FIGchar({"   ", "  x", " xx", "xx "});
        CHECK(amount(output, fc, '$', 0xbf) == 0);

        output = {"", "", "", ""};
        fc = FIGchar({"   ", "  x", " xx", "   "});
        CHECK(amount(output, fc, '$', 0xbf) == 1);

        output = {"xxx ", "xx  ", "x   ", "    "};
        fc = FIGchar({"   y", "  yy", " yyy", "yyyy"});
        CHECK(amount(output, fc, '$', 0xbf) == 4);

        output = {"xxxx ", "xxx  ", "xx   ", "x    "};
        fc = FIGchar({"   x", "  xx", " xxx", "xxxx"});
        CHECK(amount(output, fc, '$', 0xbf) == 5);
    }

    TEST_CASE("smusher::trim") {
        Lines output{"12345", "abcde"};
        trim(output, 3);
        CHECK(output == Lines{"123", "abc"});
    }
}
