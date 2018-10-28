#include "doctest.h"
#include "../src/figchar.cpp"

TEST_SUITE("figchar") {
    TEST_CASE("figchar::construct_from_height") {
        FIGchar c(3);
        auto l = c.get();
        REQUIRE(l[0] == "");
        REQUIRE(l[1] == "");
        REQUIRE(l[2] == "");
    }

    TEST_CASE("figchar::construct_from_lines") {
        FIGchar c({"123", "456", "789"});
        auto l = c.get();
        REQUIRE(l[0] == "123");
        REQUIRE(l[1] == "456");
        REQUIRE(l[2] == "789");
    }

    TEST_CASE("figchar::construct_from_lines_unaligned") {
        Lines v{"123", "456", "7890"};
        REQUIRE_THROWS_AS(FIGchar c(v), std::runtime_error);
    }

    TEST_CASE("figchar::operator==") {
        FIGchar a({"123", "456", "789"});
        FIGchar b({"123", "456", "789"});
        FIGchar c({"123", "456", "780"});
        REQUIRE(a == b);
        REQUIRE(a != c);
    }
}
