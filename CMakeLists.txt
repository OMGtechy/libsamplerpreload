cmake_minimum_required(VERSION 3.16)
project(samplerpreload VERSION 0.0.1 LANGUAGES CXX)

enable_testing()

add_compile_options(-Wall -Wextra -pedantic -Werror)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsanitize=undefined -fno-omit-frame-pointer -g3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=undefined -fno-omit-frame-pointer -g3")

include(FetchContent)
FetchContent_Declare(
    libsignalsampler
    GIT_REPOSITORY https://github.com/OMGtechy/libsignalsampler.git
    GIT_TAG 1280c7f72c36e11a033737ebf38750eb98e6bfdb
)

FetchContent_MakeAvailable(libsignalsampler)

add_library(samplerpreload SHARED source/samplerpreload.cpp)
target_link_libraries(samplerpreload signalsampler)
