#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "culminate/culminate.h"
#include <chrono>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <numeric>

struct BenchmarkResult {
    std::string name;
    double min_ns;
    double max_ns;
    double mean_ns;
    double median_ns;
    double stddev_ns;
    double throughput_ops_sec;
    size_t iterations;
};

static std::vector<BenchmarkResult> g_results;

class Benchmark {
public:
    Benchmark(const std::string& name, size_t iterations = 10000)
        : name_(name), iterations_(iterations), warmup_(1000) {}

    template<typename Func>
    BenchmarkResult run(Func&& func) {
        for (size_t i = 0; i < warmup_; ++i) {
            func();
        }

        std::vector<double> times;
        times.reserve(iterations_);

        for (size_t i = 0; i < iterations_; ++i) {
            auto start = std::chrono::high_resolution_clock::now();
            func();
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration<double, std::nano>(end - start);
            times.push_back(duration.count());
        }

        std::sort(times.begin(), times.end());

        double sum = std::accumulate(times.begin(), times.end(), 0.0);
        double mean = sum / times.size();
        
        double sq_sum = 0.0;
        for (const auto& t : times) {
            sq_sum += (t - mean) * (t - mean);
        }
        double stddev = std::sqrt(sq_sum / times.size());

        double median = times[times.size() / 2];
        double throughput = 1e9 / median;

        return BenchmarkResult{
            name_,
            times.front(),
            times.back(),
            mean,
            median,
            stddev,
            throughput,
            iterations_
        };
    }

private:
    std::string name_;
    size_t iterations_;
    size_t warmup_;
};

void saveResultsJson(const std::vector<BenchmarkResult>& results, const std::string& filename) {
    std::ofstream file(filename);
    file << "{\n";
    file << "  \"benchmarks\": [\n";
    for (size_t i = 0; i < results.size(); ++i) {
        const auto& r = results[i];
        file << "    {\n";
        file << "      \"name\": \"" << r.name << "\",\n";
        file << "      \"min_ns\": " << std::fixed << std::setprecision(2) << r.min_ns << ",\n";
        file << "      \"max_ns\": " << r.max_ns << ",\n";
        file << "      \"mean_ns\": " << r.mean_ns << ",\n";
        file << "      \"median_ns\": " << r.median_ns << ",\n";
        file << "      \"stddev_ns\": " << r.stddev_ns << ",\n";
        file << "      \"throughput_ops_sec\": " << r.throughput_ops_sec << ",\n";
        file << "      \"iterations\": " << r.iterations << "\n";
        file << "    }";
        if (i < results.size() - 1) file << ",";
        file << "\n";
    }
    file << "  ]\n";
    file << "}\n";
}

void printResult(const BenchmarkResult& r) {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  " << r.name << "\n";
    std::cout << "    Median: " << r.median_ns << " ns/op\n";
    std::cout << "    Mean:   " << r.mean_ns << " ns/op\n";
    std::cout << "    Min:    " << r.min_ns << " ns/op\n";
    std::cout << "    Max:    " << r.max_ns << " ns/op\n";
    std::cout << "    StdDev: " << r.stddev_ns << " ns/op\n";
    std::cout << "    Throughput: " << static_cast<size_t>(r.throughput_ops_sec) << " ops/sec\n";
}

TEST_CASE("Benchmark: Cell display", "[bench-cell]") {
    std::cout << "\n=== Cell Display Benchmarks ===" << std::endl;

    SECTION("Tiny cell (5 chars)") {
        culminate::Level level;
        level.column(0).width(10);
        level.indent(0);
        culminate::Row row(level);
        row.add("hello");
        
        auto result = Benchmark("Cell display 5 chars", 50000).run([&]() {
            std::ostringstream oss;
            row.display(oss);
        });
        
        printResult(result);
        g_results.push_back(result);
    }

    SECTION("Medium cell (20 chars)") {
        culminate::Level level;
        level.column(0).width(25);
        level.indent(0);
        culminate::Row row(level);
        row.add("This is a medium string");
        
        auto result = Benchmark("Cell display 20 chars", 50000).run([&]() {
            std::ostringstream oss;
            row.display(oss);
        });
        
        printResult(result);
        g_results.push_back(result);
    }

    SECTION("Center aligned cell") {
        culminate::Level level;
        level.column(0).width(20);
        level.column(0).center();
        level.indent(0);
        culminate::Row row(level);
        row.add("center");
        
        auto result = Benchmark("Cell display center", 50000).run([&]() {
            std::ostringstream oss;
            row.display(oss);
        });
        
        printResult(result);
        g_results.push_back(result);
    }

    SECTION("Right aligned cell (numeric)") {
        culminate::Level level;
        level.column(0).width(15);
        level.column(0).setNumeric(true);
        level.indent(0);
        culminate::Row row(level);
        row.add("12345");
        
        auto result = Benchmark("Cell display numeric", 50000).run([&]() {
            std::ostringstream oss;
            row.display(oss);
        });
        
        printResult(result);
        g_results.push_back(result);
    }
}

TEST_CASE("Benchmark: Row display", "[bench-row]") {
    std::cout << "\n=== Row Display Benchmarks ===" << std::endl;

    SECTION("Row with 5 columns") {
        culminate::Level level;
        level.indent(0);
        for (int i = 0; i < 5; ++i) {
            level.column(i).width(15);
        }
        culminate::Row row(level);
        row.add("one");
        row.add("two");
        row.add("three");
        row.add("four");
        row.add("five");
        
        auto result = Benchmark("Row display 5 cols", 20000).run([&]() {
            std::ostringstream oss;
            row.display(oss);
        });
        
        printResult(result);
        g_results.push_back(result);
    }

    SECTION("Row with 10 columns") {
        culminate::Level level;
        level.indent(0);
        for (int i = 0; i < 10; ++i) {
            level.column(i).width(12);
        }
        culminate::Row row(level);
        for (int i = 0; i < 10; ++i) {
            row.add("item" + std::to_string(i));
        }
        
        auto result = Benchmark("Row display 10 cols", 10000).run([&]() {
            std::ostringstream oss;
            row.display(oss);
        });
        
        printResult(result);
        g_results.push_back(result);
    }

    SECTION("Row with 50 columns") {
        culminate::Level level;
        level.indent(0);
        for (int i = 0; i < 10; ++i) {  // Max 12 columns
            level.column(i).width(10);
        }
        culminate::Row row(level);
        for (int i = 0; i < 10; ++i) {  // Max 12 columns
            row.add("c" + std::to_string(i));
        }
        
        auto result = Benchmark("Row display 10 cols (50 test)", 5000).run([&]() {
            std::ostringstream oss;
            row.display(oss);
        });
        
        printResult(result);
        g_results.push_back(result);
    }

    SECTION("Row with 100 columns") {
        culminate::Level level;
        level.indent(0);
        for (int i = 0; i < 12; ++i) {  // Max 12 columns
            level.column(i).width(8);
        }
        culminate::Row row(level);
        for (int i = 0; i < 12; ++i) {  // Max 12 columns
            row.add("x" + std::to_string(i % 12));
        }
        
        auto result = Benchmark("Row display 12 cols (100 test)", 2000).run([&]() {
            std::ostringstream oss;
            row.display(oss);
        });
        
        printResult(result);
        g_results.push_back(result);
    }
}

TEST_CASE("Benchmark: String trim", "[bench-trim]") {
    std::cout << "\n=== String Trim Benchmarks ===" << std::endl;

    std::vector<std::string> strings = {
        "  hello  ",
        "   trimmed   ",
        "no spaces",
        "     lots of space     ",
        "\t\ttabbed\t\t",
        "\n\nnewlines\n\n"
    };

    std::vector<std::string> strings_copy = strings;

    SECTION("Trim 6 strings") {
        auto result = Benchmark("Trim 6 strings", 50000).run([&]() {
            for (auto& s : strings_copy) {
                s = culminate::trim(s);
            }
        });
        
        printResult(result);
        g_results.push_back(result);
    }

    SECTION("Trim single short string") {
        std::string s = "  hi  ";
        auto result = Benchmark("Trim short string", 100000).run([&]() {
            culminate::trim(s);
        });
        
        printResult(result);
        g_results.push_back(result);
    }

    SECTION("Trim single long string") {
        std::string s = "   This is a much longer string with extra whitespace   ";
        auto result = Benchmark("Trim long string", 100000).run([&]() {
            culminate::trim(s);
        });
        
        printResult(result);
        g_results.push_back(result);
    }
}

TEST_CASE("Benchmark: Configuration lookups", "[bench-config]") {
    std::cout << "\n=== Configuration Lookup Benchmarks ===" << std::endl;

    SECTION("Justify lookup") {
        culminate::Level level;
        level.column(0).width(10);
        level.column(0).setNumeric(true);
        level.indent(0);
        culminate::Row row(level);
        row.add("test");
        
        auto result = Benchmark("Justify lookup", 100000).run([&]() {
            auto& conf = level.column(0);
            (void)conf.justify();
        });
        
        printResult(result);
        g_results.push_back(result);
    }

    SECTION("isCenter check") {
        culminate::Level level;
        level.column(0).width(10);
        level.column(0).center();
        
        auto result = Benchmark("isCenter check", 100000).run([&]() {
            auto& conf = level.column(0);
            (void)conf.isCenter();
        });
        
        printResult(result);
        g_results.push_back(result);
    }

    SECTION("width() call") {
        culminate::Level level;
        level.column(0).width(10);
        
        auto result = Benchmark("width() call", 100000).run([&]() {
            auto& conf = level.column(0);
            (void)conf._width;
        });
        
        printResult(result);
        g_results.push_back(result);
    }
}

TEST_CASE("Benchmark: Surge operations", "[bench-surge]") {
    std::cout << "\n=== Surge Benchmarks ===" << std::endl;

    SECTION("Create and render surge (10 levels, 5 cols each)") {
        auto result = Benchmark("Surge render 10x5", 5000).run([&]() {
            culminate::Surge surge;
            for (int i = 0; i < 10; ++i) {
                auto& level = surge.level(i);
                for (int j = 0; j < 5; ++j) {
                    level.column(j).width(10);
                }
            }
            surge << "a" << "b" << "c" << "d" << "e" << culminate::Level::next;
            surge << "1" << "2" << "3" << "4" << "5" << culminate::Level::next;
            surge << "x" << "y" << "z" << "w" << "v" << culminate::Level::next;
            surge.flush();
        });
        
        printResult(result);
        g_results.push_back(result);
    }

    SECTION("Create and render surge (50 levels, 10 cols each)") {
        auto result = Benchmark("Surge render 50x10", 1000).run([&]() {
            culminate::Surge surge;
            for (int i = 0; i < 50; ++i) {
                auto& level = surge.level(i);
                for (int j = 0; j < 10; ++j) {
                    level.column(j).width(8);
                }
            }
            surge << "a" << "b" << "c" << "d" << "e" << "f" << "g" << "h" << "i" << "j" << culminate::Level::next;
            surge << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "0" << culminate::Level::next;
            surge.flush();
        });
        
        printResult(result);
        g_results.push_back(result);
    }
}

TEST_CASE("Benchmark: Mixed alignment row", "[bench-mixed]") {
    std::cout << "\n=== Mixed Alignment Benchmarks ===" << std::endl;

    SECTION("Row with left, center, right columns") {
        culminate::Level level;
        level.indent(0);
        level.column(0).width(15);
        level.column(1).width(15);
        level.column(1).center();
        level.column(2).width(15);
        level.column(2).setNumeric(true);
        
        culminate::Row row(level);
        row.add("left");
        row.add("center");
        row.add("12345");
        
        auto result = Benchmark("Mixed alignment row", 20000).run([&]() {
            std::ostringstream oss;
            row.display(oss);
        });
        
        printResult(result);
        g_results.push_back(result);
    }
}

TEST_CASE("Benchmark: Decorator application", "[bench-decorator]") {
    std::cout << "\n=== Decorator Benchmarks ===" << std::endl;

    SECTION("Fixed code decorator (asterisk)") {
        culminate::Level level;
        level.column(0).width(10);
        level.column(0).apply(culminate::decorator::code('*'));
        level.indent(0);
        culminate::Row row(level);
        row.add("test");
        
        auto result = Benchmark("Decorator code asterisk", 50000).run([&]() {
            std::ostringstream oss;
            row.display(oss);
        });
        
        printResult(result);
        g_results.push_back(result);
    }

    SECTION("Fixed code decorator (hash)") {
        culminate::Level level;
        level.column(0).width(10);
        level.column(0).apply(culminate::decorator::code('#'));
        level.indent(0);
        culminate::Row row(level);
        row.add("value");
        
        auto result = Benchmark("Decorator code hash", 50000).run([&]() {
            std::ostringstream oss;
            row.display(oss);
        });
        
        printResult(result);
        g_results.push_back(result);
    }

    SECTION("Conditional code (empty string)") {
        culminate::Level level;
        level.column(0).width(10);
        level.column(0).apply(culminate::decorator::conditionalCode(
            [](const std::string& s) { return s.empty(); },
            "N/A"
        ));
        level.indent(0);
        culminate::Row row(level);
        row.add("");
        
        auto result = Benchmark("Decorator conditional empty", 50000).run([&]() {
            std::ostringstream oss;
            row.display(oss);
        });
        
        printResult(result);
        g_results.push_back(result);
    }

    SECTION("Conditional code (non-empty)") {
        culminate::Level level;
        level.column(0).width(10);
        level.column(0).apply(culminate::decorator::conditionalCode<std::string>(
            [](const std::string& s) { return !s.empty(); },
            std::string("OK"), std::string("EMPTY")
        ));
        level.indent(0);
        culminate::Row row(level);
        row.add("data");
        
        auto result = Benchmark("Decorator conditional non-empty", 50000).run([&]() {
            std::ostringstream oss;
            row.display(oss);
        });
        
        printResult(result);
        g_results.push_back(result);
    }

    SECTION("Pre and post decorators") {
        culminate::Level level;
        level.column(0).width(10);
        auto preDecorator = [](std::ostream& os, const std::string&) -> std::ostream& { return os << "["; };
        auto postDecorator = [](std::ostream& os, const std::string&) -> std::ostream& { return os << "]"; };
        level.column(0).apply(
            culminate::decorator::Tool(preDecorator),
            culminate::decorator::Tool(postDecorator)
        );
        level.indent(0);
        culminate::Row row(level);
        row.add("text");
        
        auto result = Benchmark("Decorator pre-post", 50000).run([&]() {
            std::ostringstream oss;
            row.display(oss);
        });
        
        printResult(result);
        g_results.push_back(result);
    }

    SECTION("Multiple decorators chained") {
        culminate::Level level;
        level.column(0).width(15);
        level.column(0).apply(culminate::decorator::code('*'));
        level.column(0).apply(culminate::decorator::code('+'));
        level.indent(0);
        culminate::Row row(level);
        row.add("item");
        
        auto result = Benchmark("Decorator chained", 50000).run([&]() {
            std::ostringstream oss;
            row.display(oss);
        });
        
        printResult(result);
        g_results.push_back(result);
    }

    SECTION("Save all results") {
        std::cout << "\n=== Saving baseline results ===" << std::endl;
        saveResultsJson(g_results, "benchmarks_baseline.json");
        std::cout << "Saved " << g_results.size() << " benchmark results to benchmarks_baseline.json" << std::endl;
    }
}
