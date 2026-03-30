#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "culminate/culminate.h"

TEST_CASE("Level construction", "[level]") {
    SECTION("Default level") {
        culminate::Level level;
        REQUIRE(level.size() == 0);
        REQUIRE(level.separator() == "  ");
        REQUIRE(level.indent().empty());
    }

    SECTION("Level with indent") {
        culminate::Level level(2);
        REQUIRE(level.indent() == "    ");
    }

    SECTION("Level with custom separator") {
        culminate::Level level(0, "|");
        REQUIRE(level.separator() == "|");
    }
}

TEST_CASE("Level indent", "[level]") {
    culminate::Level level;
    
    SECTION("Set indent") {
        level.indent(5);
        REQUIRE(level.indent().size() == 5);
    }
}

TEST_CASE("Level column configuration", "[level]") {
    culminate::Level level;

    SECTION("Column access creates new column") {
        REQUIRE(level.size() == 0);
        level.column(0);
        REQUIRE(level.size() == 1);
    }

    SECTION("Column width") {
        level.column(0).width(10);
        REQUIRE(level.column(0)._width == 10);
    }

    SECTION("Column visibility") {
        level.column(0).hide();
        REQUIRE(level.column(0).visible() == false);
    }
}

TEST_CASE("Level width updates", "[level]") {
    culminate::Level level;
    level.column(0).width(5);

    SECTION("updateWidths increases width") {
        std::vector<std::string> values = {"hello", "world"};
        level.updateWidths(values);
        REQUIRE(level.column(0)._width == 5);
        REQUIRE(level.column(1)._width == 5);
    }
}
