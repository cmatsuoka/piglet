#include "doctest.h"
#include "../src/wrapper.cpp"

TEST_SUITE("wrapper") {
    TEST_CASE("wrapper::padding") {
        FIGline v = FIGline{"x", "x"};
        CHECK(add_pad(v, 0) == FIGline{"x", "x"});
        CHECK(add_pad(v, 4) == FIGline{"    x", "    x"});
    }
}
