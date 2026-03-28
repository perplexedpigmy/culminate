cmake_minimum_required(VERSION 3.14)
project(culminate VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable compiler warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
elseif(MSVC)
  add_compile_options(/W4)
endif()

include(FetchContent)
FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.4.0
)
set(CATCH2_INCLUDE_DIR "${catch2_BINARY_DIR}/generated-includes" CACHE PATH "Path to Catch2 include")
FetchContent_MakeAvailable(catch2)

add_library(culminate INTERFACE)
target_include_directories(culminate INTERFACE 
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

enable_testing()

add_subdirectory(tests)
add_subdirectory(examples)
