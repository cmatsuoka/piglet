#include "doctest.h"
#include "../src/figfont.cpp"

namespace {

    const std::string top_dir(TOP_DIR);

}  // namespace


TEST_SUITE("figfont") {
    TEST_CASE("figfont::font_load") {
        auto font = FIGfont(top_dir + "/fonts/standard.flf");
        FIGchar c;

        c = FIGchar({L" $",
                     L" $",
                     L" $",
                     L" $",
                     L" $",
                     L" $"});
        CHECK(font.get(' ') == c);

        c = FIGchar({LR"(     _    )",
                     LR"(    / \   )",
                     LR"(   / _ \  )",
                     LR"(  / ___ \ )",
                     LR"( /_/   \_\)",
                     LR"(          )"});
        CHECK(font.get('A') == c);

        c = FIGchar({LR"(   ___ )",
                     LR"(  / _ \)",
                     LR"( | |/ /)",
                     LR"( | |\ \)",
                     LR"( | ||_/)",
                     LR"( |_|   )"});
        CHECK(font.get(223) == c);

        c = FIGchar({LR"#(   _____))#",
                     LR"#(  /_ ___/)#",
                     LR"#(  / _ \  )#",
                     LR"#( | (_) | )#",
                     LR"#( $\___/$ )#",
                     LR"#(         )#"});
        CHECK(font.get(3232) == c);
    }

    TEST_CASE("figfont::get_tab") {
        auto font = FIGfont(top_dir + "/fonts/standard.flf");
        FIGline v{L" $",
                  L" $",
                  L" $",
                  L" $",
                  L" $",
                  L" $"};
        REQUIRE(font.get('\t').get() == v);
    }
}
