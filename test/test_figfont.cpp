#include "doctest.h"
#include "../src/figfont.cpp"

TEST_SUITE("figfont") {
    TEST_CASE("figfont::font_load") {
        auto font = FIGfont("../fonts/standard.flf");
        FIGchar c;

        c = FIGchar({" $",
                     " $",
                     " $",
                     " $",
                     " $",
                     " $"});
        CHECK(font.get(' ') == c);

        c = FIGchar({R"(     _    )",
                     R"(    / \   )",
                     R"(   / _ \  )",
                     R"(  / ___ \ )",
                     R"( /_/   \_\)",
                     R"(          )"});
        CHECK(font.get('A') == c);

        c = FIGchar({R"(   ___ )",
                     R"(  / _ \)",
                     R"( | |/ /)",
                     R"( | |\ \)",
                     R"( | ||_/)",
                     R"( |_|   )"});
        CHECK(font.get(223) == c);

        c = FIGchar({R"#(   _____))#",
                     R"#(  /_ ___/)#",
                     R"#(  / _ \  )#",
                     R"#( | (_) | )#",
                     R"#( $\___/$ )#",
                     R"#(         )#"});
        CHECK(font.get(3232) == c);
    }
}
