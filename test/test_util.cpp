#include "doctest.h"
#include "../src/util.cpp"

TEST_SUITE("util") {
    TEST_CASE("util::trim_right") {
        std::wstring x;
        x = L""; util::trim_right(x); CHECK(x == L"");
        x = L" "; util::trim_right(x); CHECK(x == L"");
        x = L"\t"; util::trim_right(x); CHECK(x == L"");
        x = L" abc"; util::trim_right(x); CHECK(x == L" abc");
        x = L"abc "; util::trim_right(x); CHECK(x == L"abc");
        x = L"abc  \t "; util::trim_right(x); CHECK(x == L"abc");
    }

    TEST_CASE("util::starts_with") {
        CHECK(!util::starts_with(L"", L"abc"));
        CHECK(util::starts_with(L"abc", L"abc"));
        CHECK(util::starts_with(L"abc123", L"abc"));
        CHECK(!util::starts_with(L"abc123", L"bc1"));
    }

    TEST_CASE("util::split_whitespace") {
        CHECK(util::split_whitespace(L"") == std::vector<std::wstring>{});
        CHECK(util::split_whitespace(L" ") == std::vector<std::wstring>{});
        CHECK(util::split_whitespace(L"  ") == std::vector<std::wstring>{});
        CHECK(util::split_whitespace(L"a") == std::vector<std::wstring>{L"a"});
        CHECK(util::split_whitespace(L"  a") == std::vector<std::wstring>{L"a"});
        CHECK(util::split_whitespace(L"  a   b ") == std::vector<std::wstring>{L"a", L"b"});
        CHECK(util::split_whitespace(L"abc de f") == std::vector<std::wstring>{L"abc", L"de", L"f"});
    }
}
