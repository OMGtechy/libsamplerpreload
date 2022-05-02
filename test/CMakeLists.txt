cmake_minimum_required(VERSION 3.16)
project(samplerpreload-test VERSION 0.0.1 LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
    catch
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v2.13.9
)

FetchContent_MakeAvailable(catch)

add_executable(samplerpreload-test source/samplerpreload-test.cpp source/settings-test.cpp source/trace-test.cpp)
target_compile_options(samplerpreload-test PRIVATE -DCATCH_CONFIG_DISABLE_MATCHERS -DCATCH_CONFIG_FAST_COMPILE)
target_include_directories(samplerpreload-test PUBLIC include)
target_link_libraries(samplerpreload-test Catch2::Catch2 samplerpreload)
