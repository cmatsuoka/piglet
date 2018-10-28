#include "doctest.h"
#include "../src/charsmush.cpp"

TEST_SUITE("charsmush") {
    TEST_CASE("charsmush::rule_1") {
        CHECK(rule_1('-', '-', 0) == 0);
        CHECK(rule_1('-', 'x', 0) == 0);
        CHECK(rule_1('-', '-', SmushEqual) == '-');
        CHECK(rule_1('-', 'x', SmushEqual) == 0);
    }

    TEST_CASE("charsmush::rule_2") {
        char list[] = { '|', '/', '\\', '[', ']', '{', '}', '(', ')', '<', '>' };
        for (auto x : list) {
            CHECK(rule_2('_', x, 0) == 0);
            CHECK(rule_2(x, '_', 0) == 0);
            CHECK(rule_2('_', x, SmushUnderline) == x);
            CHECK(rule_2(x, '_', SmushUnderline) == x);
        }
        CHECK(rule_2('_', 'x', 0) == 0);
        CHECK(rule_2('x', '_', 0) == 0);
        CHECK(rule_2('_', 'x', SmushUnderline) == 0);
        CHECK(rule_2('x', '_', SmushUnderline) == 0);
    }

    TEST_CASE("charsmush::rule_3") {
        char list[] = { '|', '/', '\\', '[', ']', '{', '}', '(', ')', '<', '>' };
        for (auto x : list) {
            for (auto y : list) {
                CHECK(rule_3(x, y, 0) == 0);
            }
        }
        CHECK(rule_3('|', '|', SmushHierarchy) == 0);
        for (auto x = std::begin(list) + 1; x != std::end(list); x++) {
            CHECK(rule_3('|', *x, SmushHierarchy) == *x);
        }
        for (int i = 0; i < 4; i++) {
            int idx = 3 + i*2;
            auto x = list[idx];
            for (int j = idx + 2; j < sizeof(list); j++) {
                auto y = list[j];
                CHECK(rule_3(x, y, SmushHierarchy) == y);
                CHECK(rule_3(y, x, SmushHierarchy) == y);
            }
        }
    }

    TEST_CASE("charsmush::rule_4") {
        CHECK(rule_4('[', ']', 0) == 0);
        CHECK(rule_4(']', '[', 0) == 0);
        CHECK(rule_4('{', '}', 0) == 0);
        CHECK(rule_4('}', '{', 0) == 0);
        CHECK(rule_4('(', ')', 0) == 0);
        CHECK(rule_4(')', '(', 0) == 0);
        CHECK(rule_4('[', ']', SmushPair) == '|');
        CHECK(rule_4(']', '[', SmushPair) == '|');
        CHECK(rule_4('{', '}', SmushPair) == '|');
        CHECK(rule_4('}', '{', SmushPair) == '|');
        CHECK(rule_4('(', ')', SmushPair) == '|');
        CHECK(rule_4(')', '(', SmushPair) == '|');
        CHECK(rule_4('(', 'x', SmushPair) == 0);
        CHECK(rule_4('(', '}', SmushPair) == 0);
        CHECK(rule_4('(', ']', SmushPair) == 0);
        CHECK(rule_4('(', '(', SmushPair) == 0);
    }

    TEST_CASE("charsmush::rule_5") {
        CHECK(rule_5('/', '\\', 0) == 0);
        CHECK(rule_5('\\', '/', 0) == 0);
        CHECK(rule_5('>', '<' , 0) == 0);
        CHECK(rule_5('<', '>' , 0) == 0);
        CHECK(rule_5('/', '\\', SmushBigX) == '|');
        CHECK(rule_5('\\', '/', SmushBigX) == 'Y');
        CHECK(rule_5('>', '<' , SmushBigX) == 'X');
        CHECK(rule_5('<', '>' , SmushBigX) == 0);
    }
}
