#include "doctest.h"
#include "../src/strsmush.cpp"

TEST_SUITE("strsmush") {
    TEST_CASE("strsmush::amount") {
        CHECK(amount("", "", '$', 0xbf) == 0);

        CHECK(amount("", "    ", '$', 0xbf) == 4);
        CHECK(amount("", "   y", '$', 0xbf) == 3);

        CHECK(amount("    ", "    ", '$', 0xbf) == 8);
        CHECK(amount("x   ", "    ", '$', 0xbf) == 7);
        CHECK(amount("xx  ", "    ", '$', 0xbf) == 6);
        CHECK(amount("xxx ", "    ", '$', 0xbf) == 5);
        CHECK(amount("xxxx", "    ", '$', 0xbf) == 4);

        CHECK(amount("    ", "   y", '$', 0xbf) == 7);
        CHECK(amount("x   ", "   y", '$', 0xbf) == 6);
        CHECK(amount("xx  ", "   y", '$', 0xbf) == 5);
        CHECK(amount("xxx ", "   y", '$', 0xbf) == 4);
        CHECK(amount("xxxx", "   y", '$', 0xbf) == 3);

        CHECK(amount("    ", "  yy", '$', 0xbf) == 6);
        CHECK(amount("x   ", "  yy", '$', 0xbf) == 5);
        CHECK(amount("xx  ", "  yy", '$', 0xbf) == 4);
        CHECK(amount("xxx ", "  yy", '$', 0xbf) == 3);
        CHECK(amount("xxxx", "  yy", '$', 0xbf) == 2);

        CHECK(amount("    ", " yyy", '$', 0xbf) == 5);
        CHECK(amount("x   ", " yyy", '$', 0xbf) == 4);
        CHECK(amount("xx  ", " yyy", '$', 0xbf) == 3);
        CHECK(amount("xxx ", " yyy", '$', 0xbf) == 2);
        CHECK(amount("xxxx", " yyy", '$', 0xbf) == 1);

        CHECK(amount("    ", "yyyy", '$', 0xbf) == 4);
        CHECK(amount("x   ", "yyyy", '$', 0xbf) == 3);
        CHECK(amount("xx  ", "yyyy", '$', 0xbf) == 2);
        CHECK(amount("xxx ", "yyyy", '$', 0xbf) == 1);
        CHECK(amount("xxxx", "yyyy", '$', 0xbf) == 0);

        CHECK(amount("x", "y", '$', 0xbf) == 0);
        CHECK(amount("x", "x", '$', 0xbf) == 1);     // rule 1
        CHECK(amount("<", ">", '$', 0xbf) == 0);
        CHECK(amount("_", "/", '$', 0xbf) == 1);     // rule 2
        CHECK(amount("/", "_", '$', 0xbf) == 1);     // rule 2
        CHECK(amount("[", "{", '$', 0xbf) == 1);     // rule 3
        CHECK(amount("[", "]", '$', 0xbf) == 1);     // rule 4
        CHECK(amount(">", "<", '$', 0xbf) == 1);     // rule 5
        CHECK(amount("[ ", " {", '$', 0xbf) == 3);   // rule 3 + spacing
    }

    TEST_CASE("strsmush::smush") {
        CHECK(smush("123! ", "xy", 1, '$', 0xbf) == "123!xy");
        CHECK(smush("123> ", "<y", 2, '$', 0xbf) == "123Xy");
        CHECK(smush("123! ", "   xy", 5, '$', 0xbf) == "123xy");
        CHECK(smush("123/ ", "   /y", 5, '$', 0xbf) == "123/y");
        CHECK(smush("", "   y", 3, '$', 0xbf) == "y");
        CHECK(smush("", "      ", 1, '$', 0xbf) == "     ");
    }
}
