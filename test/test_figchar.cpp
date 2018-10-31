#include "doctest.h"
#include "../src/figchar.cpp"

TEST_SUITE("figchar") {
    TEST_CASE("figchar::construct_from_height") {
        FIGchar c(3);
        REQUIRE(c.get() == FIGline{L"", L"", L""});
    }

    TEST_CASE("figchar::construct_from_lines") {
        FIGchar c({L"123", L"456", L"789"});
        auto l = c.get();
        REQUIRE(c.get() == FIGline{L"123", L"456", L"789"});
    }

    TEST_CASE("figchar::construct_from_lines_unaligned") {
        REQUIRE_THROWS_AS(FIGchar c({L"123", L"456", L"7890"}), std::runtime_error);
    }

    TEST_CASE("figchar::operator==") {
        FIGchar a({L"123", L"456", L"789"});
        FIGchar b({L"123", L"456", L"789"});
        FIGchar c({L"123", L"456", L"780"});
        REQUIRE(a == b);
        REQUIRE(a != c);
    }
}
