#include "doctest.h"
#include "../src/wrapper.cpp"

TEST_SUITE("wrapper") {
    TEST_CASE("wrapper::padding") {
        FIGline v = FIGline{L"x", L"x"};
        CHECK(add_pad(v, 0) == FIGline{L"x", L"x"});
        CHECK(add_pad(v, 4) == FIGline{L"    x", L"    x"});
    }
}
