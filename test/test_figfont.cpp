#include "doctest.h"
#include "../src/figfont.cpp"

namespace {

    const std::string top_dir(TOP_DIR);

}  // namespace


TEST_SUITE("figfont") {
    TEST_CASE("figfont::font_load") {
        auto font = FIGfont(top_dir + "/fonts/standard.flf");
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

    TEST_CASE("figfont::get_tab") {
        auto font = FIGfont(top_dir + "/fonts/standard.flf");
        FIGline v{" $",
                " $",
                " $",
                " $",
                " $",
                " $"};
        REQUIRE(font.get('\t').get() == v);
    }
}
