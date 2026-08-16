cmake_minimum_required(VERSION 3.14)

project(CppTetris LANGUAGES CXX)

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

include(FetchContent)

if(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux" OR
   NOT CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|amd64|AMD64)$")
    message(FATAL_ERROR "The bundled raylib package only supports Linux x86_64")
endif()

FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/releases/download/5.0/raylib-5.0_linux_amd64.tar.gz
    URL_HASH SHA256=5724b8d89c7cedd0c582d022f195169fb3fc27646dac376238da7a2df39aa59c
)
FetchContent_MakeAvailable(raylib)

add_library(raylib_binary STATIC IMPORTED)
set_target_properties(raylib_binary PROPERTIES
    IMPORTED_LOCATION "${raylib_SOURCE_DIR}/lib/libraylib.a"
    INTERFACE_INCLUDE_DIRECTORIES "${raylib_SOURCE_DIR}/include"
)

# System libraries required by the precompiled static raylib archive.
target_link_libraries(raylib_binary INTERFACE GL m pthread dl rt X11)

add_executable(CppTetris
    src/main.cpp
    src/piece.cpp
)

target_compile_features(CppTetris PRIVATE cxx_std_20)
target_link_libraries(CppTetris PRIVATE raylib_binary)
