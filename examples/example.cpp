#include "culminate/culminate.h"
#include "rang.hpp"
#include <iostream>

using namespace culminate::literal;
using namespace std;
using namespace rang;

int main()
{
    cout << fg::blue << "=== Example 1: Employee Table ===" << fg::reset << endl << endl;

    {
        culminate::Surge s;
        s.title({"ID", "Name", "Department", "Salary", "Active"});

        // Column 0: Right-aligned (numeric)
        s.level(0).column(0).setNumeric(true);
        
        // Column 1: Center-aligned
        s.level(0).column(1).center();
        
        // Column 3: Conditional formatting with two colors
        s.level(0).column(3).apply(
            culminate::decorator::conditionalCode(
                [](int value) { return value >= 80000; },
                fg::green, fg::red
            ),
            culminate::decorator::code(rang::style::reset)
        );
        
        // Column 4: Boolean status with color
        s.level(0).column(4).apply(
            culminate::decorator::conditionalCode(
                [](bool val) { return val; },
                fg::green, fg::red
            ),
            culminate::decorator::code(rang::style::reset)
        );

        // Add employee data
        s << 1 << "Alice"   << "Engineering" << 85000  << true  << endl;
        s << 2 << "Bob"     << "Marketing"   << 65000  << false << endl;
        s << 3 << "Charlie" << "Engineering" << 95000  << true  << endl;
        s << 4 << "Diana"   << "Sales"       << 72000  << true  << endl;
        s << 5 << "Eve"      << "HR"          << 55000  << false << endl;
    }

    cout << fg::blue << "\n=== Example 2: Sales Report ===" << fg::reset << endl << endl;

    {
        culminate::Surge s;
        s.title({"Region", "Product", "Sales", "Target", "Status"});
        
        // Numeric columns
        s.level(0).column(2).setNumeric(true);
        s.level(0).column(3).setNumeric(true);
        
        // Conditional: target met or not
        s.level(0).column(4).apply(
            culminate::decorator::conditionalCode(
                [](int met) { return met > 0; },
                fg::green, fg::red
            ),
            culminate::decorator::code(rang::style::reset)
        );

        // Sales data
        s << "North" << "Widgets" << 1500 << 1000 << 1 << endl;
        s << "North" << "Gadgets" <<  800 << 1000 << 0 << endl;
        s << "South" << "Widgets" << 2000 << 1500 << 1 << endl;
        s << "South" << "Gadgets" << 1200 << 1500 << 0 << endl;
        s << "East"  << "Widgets" << 1800 << 1200 << 1 << endl;
        s << "West"  << "Widgets" <<  900 << 1200 << 0 << endl;
    }

    cout << fg::blue << "\n=== Example 3: Simple Grid ===" << fg::reset << endl << endl;

    {
        culminate::Surge s;
        s.title({"A", "B", "C"});
        
        s << 1 << 2 << 3 << endl;
        s << 4 << 5 << 6 << endl;
        s << 7 << 8 << 9 << endl;
    }

    cout << fg::blue << "\n=== Example 4: Project Status ===" << fg::reset << endl << endl;

    {
        culminate::Surge s;
        s.title({"Project", "Progress", "Team", "Risk", "Done"});
        
        // Center-aligned project names
        s.level(0).column(0).center();
        
        // Numeric for progress
        s.level(0).column(1).setNumeric(true);
        
        // Risk level (0=low, 1=medium, 2=high)
        s.level(0).column(3).apply(
            culminate::decorator::conditionalCode(
                [](int risk) { return risk == 0; },
                fg::green, fg::red
            ),
            culminate::decorator::code(rang::style::reset)
        );
        
        // Completion status
        s.level(0).column(4).apply(
            culminate::decorator::conditionalCode(
                [](bool complete) { return complete; },
                fg::green, fg::yellow
            ),
            culminate::decorator::code(rang::style::reset)
        );

        // Projects (risk: 0=low, 1=medium, 2=high)
        s << "Website Redesign" << 100 << "Team A" << 0 << true << endl;
        s << "Mobile App"       <<  75 << "Team B" << 1 << false << endl;
        s << "Database Migrate"  <<  50 << "Team C" << 1 << false << endl;
        s << "API v2"          <<  25 << "Team A" << 2 << false << endl;
    }

    cout << fg::blue << "\n=== Example 5: Multi-Level with Indentation ===" << fg::reset << endl << endl;

    {
        culminate::Surge s;
        s.title({"Level 1", "Level 2", "Value"});
        
        // Level 1 indentation
        s.level(1).indent(4);
        
        // Level 2 indentation  
        s.level(2).indent(8);
        
        // Numeric value
        s.level(2).column(2).setNumeric(true);

        // Category A
        s << "Category A" << "Item 1" << 100 << culminate::Level::next;
        s << ""           << "Item 2" << 200 << culminate::Level::next;
        s << ""           << "Item 3" << 150 << culminate::Level::next;
        s << culminate::Level::prev;

        // Category B
        s << "Category B" << "Item 1" << 300 << culminate::Level::next;
        s << ""           << "Item 2" << 250 << culminate::Level::next;
        s << culminate::Level::prev;
    }

    cout << fg::blue << "\nDone!" << fg::reset << endl;

    return 0;
}
