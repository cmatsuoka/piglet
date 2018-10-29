#include "doctest.h"
#include "../src/strsmush.cpp"

TEST_SUITE("strsmush") {
    TEST_CASE("strsmush::StringSmusher::amount") {
        CHECK(StringSmusher::amount("", "", '$', 0xbf) == 0);

        CHECK(StringSmusher::amount("", "    ", '$', 0xbf) == 4);
        CHECK(StringSmusher::amount("", "   y", '$', 0xbf) == 3);

        CHECK(StringSmusher::amount("    ", "    ", '$', 0xbf) == 8);
        CHECK(StringSmusher::amount("x   ", "    ", '$', 0xbf) == 7);
        CHECK(StringSmusher::amount("xx  ", "    ", '$', 0xbf) == 6);
        CHECK(StringSmusher::amount("xxx ", "    ", '$', 0xbf) == 5);
        CHECK(StringSmusher::amount("xxxx", "    ", '$', 0xbf) == 4);

        CHECK(StringSmusher::amount("    ", "   y", '$', 0xbf) == 7);
        CHECK(StringSmusher::amount("x   ", "   y", '$', 0xbf) == 6);
        CHECK(StringSmusher::amount("xx  ", "   y", '$', 0xbf) == 5);
        CHECK(StringSmusher::amount("xxx ", "   y", '$', 0xbf) == 4);
        CHECK(StringSmusher::amount("xxxx", "   y", '$', 0xbf) == 3);

        CHECK(StringSmusher::amount("    ", "  yy", '$', 0xbf) == 6);
        CHECK(StringSmusher::amount("x   ", "  yy", '$', 0xbf) == 5);
        CHECK(StringSmusher::amount("xx  ", "  yy", '$', 0xbf) == 4);
        CHECK(StringSmusher::amount("xxx ", "  yy", '$', 0xbf) == 3);
        CHECK(StringSmusher::amount("xxxx", "  yy", '$', 0xbf) == 2);

        CHECK(StringSmusher::amount("    ", " yyy", '$', 0xbf) == 5);
        CHECK(StringSmusher::amount("x   ", " yyy", '$', 0xbf) == 4);
        CHECK(StringSmusher::amount("xx  ", " yyy", '$', 0xbf) == 3);
        CHECK(StringSmusher::amount("xxx ", " yyy", '$', 0xbf) == 2);
        CHECK(StringSmusher::amount("xxxx", " yyy", '$', 0xbf) == 1);

        CHECK(StringSmusher::amount("    ", "yyyy", '$', 0xbf) == 4);
        CHECK(StringSmusher::amount("x   ", "yyyy", '$', 0xbf) == 3);
        CHECK(StringSmusher::amount("xx  ", "yyyy", '$', 0xbf) == 2);
        CHECK(StringSmusher::amount("xxx ", "yyyy", '$', 0xbf) == 1);
        CHECK(StringSmusher::amount("xxxx", "yyyy", '$', 0xbf) == 0);

        CHECK(StringSmusher::amount("x", "y", '$', 0xbf) == 0);
        CHECK(StringSmusher::amount("x", "x", '$', 0xbf) == 1);     // rule 1
        CHECK(StringSmusher::amount("<", ">", '$', 0xbf) == 0);
        CHECK(StringSmusher::amount("_", "/", '$', 0xbf) == 1);     // rule 2
        CHECK(StringSmusher::amount("/", "_", '$', 0xbf) == 1);     // rule 2
        CHECK(StringSmusher::amount("[", "{", '$', 0xbf) == 1);     // rule 3
        CHECK(StringSmusher::amount("[", "]", '$', 0xbf) == 1);     // rule 4
        CHECK(StringSmusher::amount(">", "<", '$', 0xbf) == 1);     // rule 5
        CHECK(StringSmusher::amount("[ ", " {", '$', 0xbf) == 3);   // rule 3 + spacing
    }

    TEST_CASE("strsmush::smush") {
        CHECK(StringSmusher::smush("123! ", "xy", 1, '$', 0xbf) == "123!xy");
        CHECK(StringSmusher::smush("123> ", "<y", 2, '$', 0xbf) == "123Xy");
        CHECK(StringSmusher::smush("123! ", "   xy", 5, '$', 0xbf) == "123xy");
        CHECK(StringSmusher::smush("123/ ", "   /y", 5, '$', 0xbf) == "123/y");
        CHECK(StringSmusher::smush("", "   y", 3, '$', 0xbf) == "y");
        CHECK(StringSmusher::smush("", "      ", 1, '$', 0xbf) == "     ");
    }
}
