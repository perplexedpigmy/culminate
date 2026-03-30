#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "culminate/culminate.h"

TEST_CASE("ltrim removes leading whitespace", "[trim]") {
    SECTION("removes spaces") {
        std::string s = "   hello";
        culminate::ltrim(s);
        REQUIRE(s == "hello");
    }

    SECTION("removes tabs") {
        std::string s = "\thello";
        culminate::ltrim(s);
        REQUIRE(s == "hello");
    }

    SECTION("removes mixed whitespace") {
        std::string s = "  \t\n hello";
        culminate::ltrim(s);
        REQUIRE(s == "hello");
    }

    SECTION("no change if no leading whitespace") {
        std::string s = "hello";
        culminate::ltrim(s);
        REQUIRE(s == "hello");
    }

    SECTION("empty string") {
        std::string s = "";
        culminate::ltrim(s);
        REQUIRE(s == "");
    }
}

TEST_CASE("rtrim removes trailing whitespace", "[trim]") {
    SECTION("removes spaces") {
        std::string s = "hello   ";
        culminate::rtrim(s);
        REQUIRE(s == "hello");
    }

    SECTION("removes tabs") {
        std::string s = "hello\t";
        culminate::rtrim(s);
        REQUIRE(s == "hello");
    }

    SECTION("removes mixed whitespace") {
        std::string s = "hello  \t\n ";
        culminate::rtrim(s);
        REQUIRE(s == "hello");
    }

    SECTION("no change if no trailing whitespace") {
        std::string s = "hello";
        culminate::rtrim(s);
        REQUIRE(s == "hello");
    }

    SECTION("empty string") {
        std::string s = "";
        culminate::rtrim(s);
        REQUIRE(s == "");
    }
}

TEST_CASE("trim removes both leading and trailing whitespace", "[trim]") {
    SECTION("removes both sides") {
        std::string s = "   hello   ";
        culminate::trim(s);
        REQUIRE(s == "hello");
    }

    SECTION("only leading") {
        std::string s = "   hello";
        culminate::trim(s);
        REQUIRE(s == "hello");
    }

    SECTION("only trailing") {
        std::string s = "hello   ";
        culminate::trim(s);
        REQUIRE(s == "hello");
    }

    SECTION("no whitespace") {
        std::string s = "hello";
        culminate::trim(s);
        REQUIRE(s == "hello");
    }

    SECTION("only whitespace") {
        std::string s = "   \t\n   ";
        culminate::trim(s);
        REQUIRE(s == "");
    }

    SECTION("empty string") {
        std::string s = "";
        culminate::trim(s);
        REQUIRE(s == "");
    }
}

TEST_CASE("isSpace helper function", "[trim]") {
    REQUIRE(culminate::isSpace(' ') == true);
    REQUIRE(culminate::isSpace('\t') == true);
    REQUIRE(culminate::isSpace('\n') == true);
    REQUIRE(culminate::isSpace('\r') == true);
    REQUIRE(culminate::isSpace('a') == false);
    REQUIRE(culminate::isSpace('1') == false);
}
