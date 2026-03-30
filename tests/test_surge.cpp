#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "culminate/culminate.h"
#include <sstream>

TEST_CASE("Surge basic operations", "[surge]") {
    SECTION("Default constructor") {
        culminate::Surge s;
        REQUIRE(true);
    }

    SECTION("Constructor with initializer list") {
        culminate::Surge s{"col1", "col2"};
        REQUIRE(true);
    }

    SECTION("Constructor with vector") {
        std::vector<std::string> cols = {"col1", "col2"};
        std::ostringstream out;
        culminate::Surge s(out, cols);
        REQUIRE(true);
    }
}

TEST_CASE("Surge output", "[surge]") {
    std::ostringstream out;
    
    SECTION("Single row with endl") {
        {
            culminate::Surge s(out, std::vector<std::string>{});
            s << 1 << "hello" << 3.14 << culminate::manipulator(std::endl);
        }
        std::string result = out.str();
        REQUIRE(result.find("1") != std::string::npos);
        REQUIRE(result.find("hello") != std::string::npos);
    }

    SECTION("Multiple rows") {
        {
            culminate::Surge s(out, std::vector<std::string>{});
            s << 1 << "a" << culminate::manipulator(std::endl);
            s << 2 << "b" << culminate::manipulator(std::endl);
        }
        std::string result = out.str();
        REQUIRE(result.find("1") != std::string::npos);
        REQUIRE(result.find("2") != std::string::npos);
    }
}

TEST_CASE("Surge level navigation", "[surge]") {
    std::ostringstream out;
    
    SECTION("nextLevel and prevLevel") {
        {
            culminate::Surge s(out, std::vector<std::string>{});
            s << 1 << "a" << culminate::manipulator(std::endl);
            s << culminate::Level::next;
            s << 2 << "b" << culminate::manipulator(std::endl);
            s << culminate::Level::prev;
            s << 3 << "c" << culminate::manipulator(std::endl);
        }
        std::string result = out.str();
        REQUIRE(result.find("1") != std::string::npos);
        REQUIRE(result.find("2") != std::string::npos);
        REQUIRE(result.find("3") != std::string::npos);
    }
}

TEST_CASE("Surge flush", "[surge]") {
    std::ostringstream out;
    
    SECTION("Manual flush with ends") {
        {
            culminate::Surge s(out, std::vector<std::string>{});
            s << 1 << "a" << culminate::manipulator(std::ends);
        }
        std::string result = out.str();
        REQUIRE(result.find("1") != std::string::npos);
    }
}

TEST_CASE("Surge title", "[surge]") {
    std::ostringstream out;
    
    SECTION("Title is rendered") {
        {
            culminate::Surge s(out, {"Col1", "Col2"});
            s << 1 << "a" << culminate::manipulator(std::endl);
        }
        std::string result = out.str();
        REQUIRE(result.find("Col1") != std::string::npos);
        REQUIRE(result.find("Col2") != std::string::npos);
    }
}
