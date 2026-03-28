#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "culminate/culminate.h"
#include <sstream>

TEST_CASE("Decorator left alignment", "[decorator]") {
    std::ostringstream oss;
    oss << std::setw(10);
    culminate::decorator::left(oss, "test");
    REQUIRE(oss.str() == "");
}

TEST_CASE("Decorator right alignment", "[decorator]") {
    std::ostringstream oss;
    oss << std::setw(10);
    culminate::decorator::right(oss, "test");
    REQUIRE(oss.str() == "");
}

TEST_CASE("Decorator center alignment", "[decorator]") {
    std::ostringstream oss;
    oss << std::setw(10);
    culminate::decorator::center(oss, "test");
    REQUIRE(oss.str() == "");
}

TEST_CASE("Decorator code", "[decorator]") {
    SECTION("code decorator applies value") {
        auto deco = culminate::decorator::code("*");
        std::ostringstream oss;
        oss << std::setw(10);
        deco(oss, "test");
        REQUIRE(oss.str().find("*") != std::string::npos);
    }
}

TEST_CASE("Decorator conditionalCode with string predicate", "[decorator]") {
    auto deco = culminate::decorator::conditionalCode(
        [](const std::string& s) { return s.length() > 3; },
        "*", "-"
    );

    std::ostringstream oss;
    oss << std::setw(10);
    deco(oss, "longer");
    REQUIRE(oss.str().find("*") != std::string::npos);
}

TEST_CASE("Decorator conditionalCode with int predicate", "[decorator]") {
    auto deco = culminate::decorator::conditionalCode(
        [](int v) { return v > 10; },
        "*", "-"
    );

    SECTION("valid int > 10") {
        std::ostringstream oss;
        oss << std::setw(10);
        deco(oss, "15");
        REQUIRE(oss.str().find("*") != std::string::npos);
    }

    SECTION("valid int <= 10") {
        std::ostringstream oss;
        oss << std::setw(10);
        deco(oss, "5");
        REQUIRE(oss.str().find("-") != std::string::npos);
    }

    SECTION("invalid int falls back to else") {
        std::ostringstream oss;
        oss << std::setw(10);
        deco(oss, "notanumber");
        REQUIRE(oss.str().find("-") != std::string::npos);
    }
}
