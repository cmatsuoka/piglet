#include "doctest.h"
#include "../src/strsmush.cpp"

TEST_SUITE("strsmush") {
    TEST_CASE("strsmush::StringSmusher::amount") {
        CHECK(StringSmusher::amount(L"", L"", '$', 0xbf) == 0);

        CHECK(StringSmusher::amount(L"", L"    ", '$', 0xbf) == 4);
        CHECK(StringSmusher::amount(L"", L"   y", '$', 0xbf) == 3);

        CHECK(StringSmusher::amount(L"    ", L"    ", '$', 0xbf) == 8);
        CHECK(StringSmusher::amount(L"x   ", L"    ", '$', 0xbf) == 7);
        CHECK(StringSmusher::amount(L"xx  ", L"    ", '$', 0xbf) == 6);
        CHECK(StringSmusher::amount(L"xxx ", L"    ", '$', 0xbf) == 5);
        CHECK(StringSmusher::amount(L"xxxx", L"    ", '$', 0xbf) == 4);

        CHECK(StringSmusher::amount(L"    ", L"   y", '$', 0xbf) == 7);
        CHECK(StringSmusher::amount(L"x   ", L"   y", '$', 0xbf) == 6);
        CHECK(StringSmusher::amount(L"xx  ", L"   y", '$', 0xbf) == 5);
        CHECK(StringSmusher::amount(L"xxx ", L"   y", '$', 0xbf) == 4);
        CHECK(StringSmusher::amount(L"xxxx", L"   y", '$', 0xbf) == 3);

        CHECK(StringSmusher::amount(L"    ", L"  yy", '$', 0xbf) == 6);
        CHECK(StringSmusher::amount(L"x   ", L"  yy", '$', 0xbf) == 5);
        CHECK(StringSmusher::amount(L"xx  ", L"  yy", '$', 0xbf) == 4);
        CHECK(StringSmusher::amount(L"xxx ", L"  yy", '$', 0xbf) == 3);
        CHECK(StringSmusher::amount(L"xxxx", L"  yy", '$', 0xbf) == 2);

        CHECK(StringSmusher::amount(L"    ", L" yyy", '$', 0xbf) == 5);
        CHECK(StringSmusher::amount(L"x   ", L" yyy", '$', 0xbf) == 4);
        CHECK(StringSmusher::amount(L"xx  ", L" yyy", '$', 0xbf) == 3);
        CHECK(StringSmusher::amount(L"xxx ", L" yyy", '$', 0xbf) == 2);
        CHECK(StringSmusher::amount(L"xxxx", L" yyy", '$', 0xbf) == 1);

        CHECK(StringSmusher::amount(L"    ", L"yyyy", '$', 0xbf) == 4);
        CHECK(StringSmusher::amount(L"x   ", L"yyyy", '$', 0xbf) == 3);
        CHECK(StringSmusher::amount(L"xx  ", L"yyyy", '$', 0xbf) == 2);
        CHECK(StringSmusher::amount(L"xxx ", L"yyyy", '$', 0xbf) == 1);
        CHECK(StringSmusher::amount(L"xxxx", L"yyyy", '$', 0xbf) == 0);

        CHECK(StringSmusher::amount(L"x", L"y", '$', 0xbf) == 0);
        CHECK(StringSmusher::amount(L"x", L"x", '$', 0xbf) == 1);     // rule 1
        CHECK(StringSmusher::amount(L"<", L">", '$', 0xbf) == 0);
        CHECK(StringSmusher::amount(L"_", L"/", '$', 0xbf) == 1);     // rule 2
        CHECK(StringSmusher::amount(L"/", L"_", '$', 0xbf) == 1);     // rule 2
        CHECK(StringSmusher::amount(L"[", L"{", '$', 0xbf) == 1);     // rule 3
        CHECK(StringSmusher::amount(L"[", L"]", '$', 0xbf) == 1);     // rule 4
        CHECK(StringSmusher::amount(L">", L"<", '$', 0xbf) == 1);     // rule 5
        CHECK(StringSmusher::amount(L"[ ", L" {", '$', 0xbf) == 3);   // rule 3 + spacing
    }

    TEST_CASE("strsmush::smush") {
        CHECK(StringSmusher::smush(L"123! ", L"xy", 1, '$', 0xbf) == L"123!xy");
        CHECK(StringSmusher::smush(L"123> ", L"<y", 2, '$', 0xbf) == L"123Xy");
        CHECK(StringSmusher::smush(L"123! ", L"   xy", 5, '$', 0xbf) == L"123xy");
        CHECK(StringSmusher::smush(L"123/ ", L"   /y", 5, '$', 0xbf) == L"123/y");
        CHECK(StringSmusher::smush(L"", L"   y", 3, '$', 0xbf) == L"y");
        CHECK(StringSmusher::smush(L"", L"      ", 1, '$', 0xbf) == L"     ");
    }
}
