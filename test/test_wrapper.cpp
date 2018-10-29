#include "doctest.h"
#include "../src/wrapper.cpp"

TEST_SUITE("wrapper") {
    TEST_CASE("wrapper::padding") {
        Lines v = Lines{"x", "x"};
        CHECK(add_pad(v, 0) == Lines{"x", "x"});
        CHECK(add_pad(v, 4) == Lines{"    x", "    x"});
    }
}
