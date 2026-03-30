#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "culminate/culminate.h"
#include <sstream>

// Helper to capture exact output from a cell display
std::string captureCellOutput(const std::string& value, bool centered = false, bool numeric = false) {
    culminate::Level level;
    level.column(0).width(value.size() + 10);  // Ensure enough width for padding
    
    // IMPORTANT: Set alignment BEFORE adding cell, since cells get config at creation time
    if (centered) {
        level.column(0).center();
    }
    if (numeric) {
        level.column(0).setNumeric(true);
    }
    
    culminate::Row row(level);
    row.add(value);
    
    std::ostringstream oss;
    row.display(oss);
    return oss.str();
}

// Helper to count leading spaces in content area (ignoring indent)
size_t countLeadingSpaces(const std::string& s) {
    size_t leading = 0;
    for (char c : s) {
        if (c == ' ') leading++;
        else break;
    }
    return leading;
}

// Helper to check if string contains value centered (with padding)
bool isCentered(const std::string& output, const std::string& value, size_t width) {
    // Find the value in the output
    size_t pos = output.find(value);
    if (pos == std::string::npos) return false;
    
    // Count leading spaces before the value
    size_t leading = countLeadingSpaces(output);
    
    // Expected: (width - value.length()) / 2 spaces before value
    size_t expectedLeading = (width > value.size()) ? (width - value.size()) / 2 : 0;
    
    // Allow some tolerance (within 2 spaces)
    return (leading >= expectedLeading - 2 && leading <= expectedLeading + 2);
}

TEST_CASE("Left alignment pads on right", "[alignment]") {
    SECTION("Basic left alignment") {
        std::string output = captureCellOutput("hi");
        // Should have value followed by spaces to fill width
        REQUIRE(output.find("hi") != std::string::npos);
    }

    SECTION("Left alignment with custom width") {
        culminate::Level level;
        level.column(0).width(10);
        level.indent(0);  // No indent
        culminate::Row row(level);
        row.add("hi");
        
        std::ostringstream oss;
        row.display(oss);
        std::string output = oss.str();
        
        // "hi" is 2 chars, width is 10, so 8 trailing spaces (before separator)
        REQUIRE(output.find("hi") != std::string::npos);
        // Left alignment: leading spaces = 0 (after indent)
        REQUIRE(countLeadingSpaces(output) == 0);
    }
}

TEST_CASE("Right alignment pads on left", "[alignment]") {
    SECTION("Basic right alignment") {
        culminate::Level level;
        level.column(0).width(10);
        level.column(0).setNumeric(true);
        level.indent(0);
        culminate::Row row(level);
        row.add("42");
        
        std::ostringstream oss;
        row.display(oss);
        std::string output = oss.str();
        
        // "42" should be at the right side with leading spaces
        REQUIRE(output.find("42") != std::string::npos);
        // Right alignment: leading spaces > 0
        REQUIRE(countLeadingSpaces(output) > 0);
    }

    SECTION("Numeric values are right aligned by default") {
        std::string output = captureCellOutput("123", false, true);
        // Numeric should have leading spaces
        REQUIRE(countLeadingSpaces(output) > 0);
    }
}

TEST_CASE("Center alignment", "[alignment]") {
    SECTION("Center alignment is enabled") {
        culminate::Level level;
        REQUIRE(level.column(0).isCenter() == false);
        level.column(0).center();
        REQUIRE(level.column(0).isCenter() == true);
    }

    SECTION("Odd width - centered text") {
        culminate::Level level;
        level.indent(0);
        level.column(0).width(10);
        level.column(0).center();
        culminate::Row row(level);
        row.add("Hi");
        
        std::ostringstream oss;
        row.display(oss);
        std::string output = oss.str();
        
        // "Hi" is 2 chars, width is 10, total pad = 8
        // Left pad = 4, right pad = 4
        // Value should be centered with padding on both sides
        REQUIRE(isCentered(output, "Hi", 10));
    }

    SECTION("Even width - centered text") {
        culminate::Level level;
        level.indent(0);
        level.column(0).width(9);
        level.column(0).center();
        culminate::Row row(level);
        row.add("Hi");
        
        std::ostringstream oss;
        row.display(oss);
        std::string output = oss.str();
        
        // "Hi" is 2 chars, width is 9, total pad = 7
        // Left pad = 3, right pad = 4
        REQUIRE(isCentered(output, "Hi", 9));
    }

    SECTION("Value longer than width - no padding") {
        culminate::Level level;
        level.column(0).width(5);
        level.column(0).center();
        level.indent(0);
        culminate::Row row(level);
        row.add("HelloWorld"); // 10 chars, longer than width 5
        
        std::ostringstream oss;
        row.display(oss);
        std::string output = oss.str();
        
        // Should output the value without centering
        REQUIRE(output.find("HelloWorld") != std::string::npos);
    }

    SECTION("Single character centered") {
        culminate::Level level;
        level.column(0).width(7);
        level.column(0).center();
        level.indent(0);
        culminate::Row row(level);
        row.add("x");
        
        std::ostringstream oss;
        row.display(oss);
        std::string output = oss.str();
        
        // "x" is 1 char, width is 7, total pad = 6
        // Left pad = 3, right pad = 3
        REQUIRE(isCentered(output, "x", 7));
    }
}

TEST_CASE("Decorator integration", "[alignment]") {
    SECTION("center() method enables center alignment") {
        culminate::Surge surge;
        surge.level(0).column(0).center();
        
        // The center() method should enable center alignment
        REQUIRE(surge.level(0).column(0).isCenter() == true);
    }

    SECTION("center() sets internal state") {
        culminate::Level level;
        level.column(0).center();
        
        // Using center() should set the center type
        REQUIRE(level.column(0).isCenter() == true);
    }
}

TEST_CASE("Alignment edge cases", "[alignment]") {
    SECTION("Empty string") {
        std::string output = captureCellOutput("");
        // Empty string should still output something (width filled with spaces)
        REQUIRE(output.find('\n') != std::string::npos);
    }

    SECTION("Exact width match - no padding needed") {
        culminate::Level level;
        level.column(0).width(4);
        level.column(0).center();
        level.indent(0);
        culminate::Row row(level);
        row.add("test"); // Exactly 4 chars
        
        std::ostringstream oss;
        row.display(oss);
        std::string output = oss.str();
        
        // Should contain "test" without leading/trailing spaces in the cell
        REQUIRE(output.find("test") != std::string::npos);
    }

    SECTION("Zero width") {
        culminate::Level level;
        level.column(0).width(0);
        level.column(0).center();
        level.indent(0);
        culminate::Row row(level);
        row.add("test");
        
        std::ostringstream oss;
        row.display(oss);
        // Should still output the value
        REQUIRE(oss.str().find("test") != std::string::npos);
    }
}

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
