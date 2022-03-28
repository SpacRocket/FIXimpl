cmake_minimum_required(VERSION 3.16.0)
set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=* --")
set(CMAKE_CXX_STANDARD 20)
project(bat)

find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)
if (CMAKE_CXX_CPPCHECK)
    list(
        APPEND CMAKE_CXX_CPPCHECK 
            "--enable=warning"
            "--inconclusive"
            "--force" 
            "--inline-suppr"
    )
endif()

add_subdirectory(bfxclient_qrpc)
add_subdirectory(bfxclient_abstract)
add_subdirectory(bfxapplication)
