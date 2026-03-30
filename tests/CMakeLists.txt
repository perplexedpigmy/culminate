include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${catch2_SOURCE_DIR}/src)
include_directories(${catch2_SOURCE_DIR}/extras)
include_directories(${CATCH2_INCLUDE_DIR})

add_executable(test_trim test_trim.cpp)
target_link_libraries(test_trim PRIVATE Catch2::Catch2WithMain)

add_executable(test_cell test_cell.cpp)
target_link_libraries(test_cell PRIVATE Catch2::Catch2WithMain)

add_executable(test_row test_row.cpp)
target_link_libraries(test_row PRIVATE Catch2::Catch2WithMain)

add_executable(test_level test_level.cpp)
target_link_libraries(test_level PRIVATE Catch2::Catch2WithMain)

add_executable(test_decorators test_decorators.cpp)
target_link_libraries(test_decorators PRIVATE Catch2::Catch2WithMain)

add_executable(test_surge test_surge.cpp)
target_link_libraries(test_surge PRIVATE Catch2::Catch2WithMain)

add_executable(test_benchmark test_benchmark.cpp)
target_link_libraries(test_benchmark PRIVATE Catch2::Catch2WithMain)

add_test(NAME TrimTests COMMAND test_trim)
add_test(NAME CellTests COMMAND test_cell)
add_test(NAME RowTests COMMAND test_row)
add_test(NAME LevelTests COMMAND test_level)
add_test(NAME DecoratorTests COMMAND test_decorators)
add_test(NAME SurgeTests COMMAND test_surge)
add_test(NAME BenchmarkTests COMMAND test_benchmark)
