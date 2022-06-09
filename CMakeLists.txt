cmake_minimum_required(VERSION 3.16)
project(samplerpreload VERSION 0.0.1 LANGUAGES CXX)

enable_testing()

add_compile_options(-fPIC -Wall -Wextra -pedantic -Werror)
add_link_options(-Wl,--no-undefined)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsanitize=undefined -fno-omit-frame-pointer -g3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=undefined -fno-omit-frame-pointer -g3")

include(FetchContent)
FetchContent_Declare(
    libsignalsampler
    GIT_REPOSITORY https://github.com/OMGtechy/libsignalsampler.git
    GIT_TAG 3ae7504d603966ecf727b183db214eb5a2dc5dec
)

FetchContent_MakeAvailable(libsignalsampler)

FetchContent_Declare(
    libsignalsafe
    GIT_REPOSITORY https://github.com/OMGtechy/libsignalsafe.git
    GIT_TAG c1e1b113cecb4ebd4433834ac4b375ddfdfd9b6e
)

FetchContent_MakeAvailable(libsignalsafe)

add_library(samplerpreload SHARED source/preload.cpp)
add_library(samplerpreload-utils source/settings.cpp source/trace.cpp source/trace-file.cpp)

target_include_directories(samplerpreload PUBLIC include)
target_include_directories(samplerpreload-utils PUBLIC include)

target_link_libraries(samplerpreload rt samplerpreload-utils)
target_link_libraries(samplerpreload-utils signalsampler signalsafe)

add_subdirectory(test)
