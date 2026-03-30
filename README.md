# culminate

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++20](https://img.shields.io/badge/C++-20-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![Build](https://github.com/anomalyco/culminate/actions/workflows/cmake.yml/badge.svg)](https://github.com/anomalyco/culminate/actions)
[![Tests](https://img.shields.io/badge/Tests-7%20passed-green.svg)](https://github.com/anomalyco/culminate/actions)

A modern C++ column output formatting library for beautiful terminal tables.

## Features

- **Multi-level Tables** - Create hierarchical data with nested levels and indentation
- **Flexible Alignment** - Left, right, and center alignment support
- **Conditional Formatting** - Color cells based on values (withrang support)
- **Numeric Detection** - Automatic right-alignment for numeric columns
- **Auto-sizing** - Columns automatically sized to fit content
- **Custom Decorators** - Apply ANSI styling, colors, and custom formatters

## Installation

### CMake (Recommended)

```cmake
add_subdirectory(culminate)
target_link_libraries(your_target PRIVATE culminate)
```

### Header-only

Simply include `include/culminate/culminate.h` in your project.

## Quick Start

```cpp
#include "culminate/culminate.h"
#include "rang.hpp"
#include <iostream>

using namespace culminate::literal;
using namespace rang;

int main() {
    culminate::Surge s;
    s.title({"Name", "Age", "Score"});
    
    // Right-align numeric columns
    s.level(0).column(1).setNumeric(true);
    s.level(0).column(2).setNumeric(true);
    
    // Conditional formatting
    s.level(0).column(2).apply(
        culminate::decorator::conditionalCode(
            [](int value) { return value >= 80; },
            fg::green, fg::red
        ),
        culminate::decorator::code(style::reset)
    );
    
    s << "Alice" << 30 << 95 << endl;
    s << "Bob"   << 25 << 65 << endl;
    s << "Carol" << 28 << 88 << endl;
    
    return 0;
}
```

Output:
```
Name    Age  Score
Alice    30     95
Bob      25     65
Carol    28     88
```

## Capabilities

### Basic Table Generation
```cpp
culminate::Surge s;
s.title({"A", "B", "C"});
s << 1 << 2 << 3 << endl;
s << 4 << 5 << 6 << endl;
```

### Multi-level Hierarchies
```cpp
culminate::Surge s;
s.title({"Category", "Item", "Value"});
s.level(1).indent(4);  // Indent level 1
s.level(2).indent(8); // Indent level 2

s << "Electronics" << "Laptop" << 999 << culminate::Level::next;
s << ""             << "Phone"  << 699 << culminate::Level::next;
s << ""             << "Tablet" << 449 << culminate::Level::prev;
s << "Furniture"   << "Chair"  << 199 << culminate::Level::next;
```

### Column Configuration
```cpp
// Right-align numeric data
s.level(0).column(0).setNumeric(true);

// Center alignment
s.level(0).column(1).center();

// Hide a column
s.level(0).column(2).hide();

// Custom width
s.level(0).column(0).width(20);
```

### Conditional Styling
```cpp
// Color based on numeric value
s.level(0).column(2).apply(
    culminate::decorator::conditionalCode(
        [](int value) { return value >= 50; },
        fg::green, fg::red
    ),
    culminate::decorator::code(style::reset)
);

// Boolean status coloring
s.level(0).column(3).apply(
    culminate::decorator::conditionalCode(
        [](bool val) { return val; },
        fg::green, fg::red
    ),
    culminate::decorator::code(style::reset)
);
```

## Requirements

- C++20 compatible compiler
- CMake 3.16+ (for building tests/examples)
- [rang](https://github.com/agauniyal/rang) (optional, for ANSI colors)

## Building

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### Run Tests
```bash
cd build
ctest --output-on-failure
```

### Run Example
```bash
./build/examples/example
```

## Performance

The library has been optimized for high-performance scenarios:

- **constexpr Configuration** - Compile-time column configuration
- **std::array Storage** - O(1) decorator lookup
- **Move Semantics** - Efficient cell data handling
- **Surge-level Caching** - Pre-computed rendering at 128 levels
- **Pre-allocated Vectors** - Minimal dynamic allocations

## Architecture

- **Surge** - Main table controller
- **Level** - Hierarchical nesting support (max 128 levels)
- **Row** - Individual data rows
- **Cell** - Single cell with value and decorators
- **Configuration** - Column styling and formatting

## License

MIT License - see [LICENSE](LICENSE) for details.
