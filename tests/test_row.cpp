#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "culminate/culminate.h"
#include <sstream>

TEST_CASE("Row basic operations", "[row]") {
    culminate::Level level;
    culminate::Row row(level);
    std::ostringstream oss;

    SECTION("Empty row displays nothing") {
        row.display(oss);
        REQUIRE(oss.str() == "\n");
    }

    SECTION("Add string value") {
        row.add("hello");
        REQUIRE(level.size() == 1);
    }

    SECTION("Add integer value") {
        row.add(42);
        REQUIRE(level.size() == 1);
        REQUIRE(level.column(0)._width == 2);
    }

    SECTION("Add double value") {
        row.add(3.14);
        REQUIRE(level.size() == 1);
    }

    SECTION("Add multiple values") {
        row.add("hello");
        row.add(42);
        row.add(3.14);
        REQUIRE(level.size() == 3);
    }
}

TEST_CASE("Row trim behavior", "[row]") {
    culminate::Level level;
    culminate::Row row(level);

    SECTION("Trims whitespace from string") {
        row.add("  hello  ");
        std::ostringstream oss;
        row.display(oss);
        REQUIRE(oss.str().find("hello") != std::string::npos);
    }
}

TEST_CASE("Row with FreeStyle literal", "[row]") {
    culminate::Level level;
    culminate::Row row(level);
    using namespace culminate::literal;

    SECTION("FreeStyle adds cell") {
        row.add(FreeStyle("test"));
        REQUIRE(level.size() == 1);
    }
}
