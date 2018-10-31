#include "doctest.h"
#include "../src/smusher.cpp"

TEST_SUITE("smusher") {
    TEST_CASE("smusher::amount") {
        FIGline output;
        FIGchar fc;

        output = {L"", L"", L"", L""};
        fc = FIGchar({L"   ", L"  x", L" xx", L"xx "});
        CHECK(amount(output, fc, '$', 0xbf) == 0);

        output = {L"", L"", L"", L""};
        fc = FIGchar({L"   ", L"  x", L" xx", L"   "});
        CHECK(amount(output, fc, '$', 0xbf) == 1);

        output = {L"xxx ", L"xx  ", L"x   ", L"    "};
        fc = FIGchar({L"   y", L"  yy", L" yyy", L"yyyy"});
        CHECK(amount(output, fc, '$', 0xbf) == 4);

        output = {L"xxxx ", L"xxx  ", L"xx   ", L"x    "};
        fc = FIGchar({L"   x", L"  xx", L" xxx", L"xxxx"});
        CHECK(amount(output, fc, '$', 0xbf) == 5);
    }

    TEST_CASE("smusher::trim") {
        FIGline output{L"12345", L"abcde"};
        trim(output, 3);
        CHECK(output == FIGline{L"123", L"abc"});
    }
}
