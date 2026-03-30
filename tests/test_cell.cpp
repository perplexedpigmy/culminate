#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "culminate/culminate.h"
#include <sstream>

TEST_CASE("Cell creation and basic operations", "[cell]") {
    culminate::Level level;
    std::ostringstream oss;

    SECTION("Cell with string value") {
        culminate::Cell cell("test", [&level]() -> culminate::Level::Configuration& {
            return level.column(0);
        });

        REQUIRE(cell.value() == "test");
        REQUIRE(cell.size() == 4);
    }

    SECTION("Cell updates level width") {
        REQUIRE(level.size() == 0);
        
        culminate::Cell cell("hello", [&level]() -> culminate::Level::Configuration& {
            return level.column(0);
        });

        REQUIRE(level.size() == 1);
        REQUIRE(level.column(0)._width == 5);
    }

    SECTION("Cell value is const") {
        culminate::Cell cell("test", [&level]() -> culminate::Level::Configuration& {
            return level.column(0);
        });

        REQUIRE(cell.value() == "test");
    }
}

TEST_CASE("Cell visibility", "[cell]") {
    culminate::Level level;
    
    SECTION("Cell is visible by default") {
        culminate::Cell cell("test", [&level]() -> culminate::Level::Configuration& {
            return level.column(0);
        });
        
        REQUIRE(level.column(0).visible() == true);
    }

    SECTION("Cell can be hidden") {
        culminate::Cell cell("test", [&level]() -> culminate::Level::Configuration& {
            return level.column(0);
        });
        
        level.column(0).hide();
        REQUIRE(level.column(0).visible() == false);
    }
}

TEST_CASE("Cell numeric detection", "[cell]") {
    culminate::Level level;
    
    SECTION("Default is not numeric") {
        culminate::Cell cell("test", [&level]() -> culminate::Level::Configuration& {
            return level.column(0);
        });
        
        auto just = level.column(0).justify();
        std::ostringstream oss;
        just(oss, "test");
        REQUIRE(oss.str() == "");
    }

    SECTION("Cell can be marked numeric") {
        culminate::Cell cell("123", [&level]() -> culminate::Level::Configuration& {
            return level.column(0);
        });
        cell.isNumeric(true);
        
        REQUIRE(level.column(0)._width == 3);
    }
}
