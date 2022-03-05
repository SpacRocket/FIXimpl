cmake_minimum_required(VERSION 3.22)
include_guard(GLOBAL)
set(CMAKE_CXX_STANDARD 20)
project(BfxApplication)

find_package(Quickfix REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(date REQUIRED)
find_package(Poco REQUIRED Foundation)

add_library(BfxApplication src/BfxApplication.cpp)

if(OPENSSL_FOUND)
target_compile_definitions(BfxApplication PUBLIC HAVE_SSL=1)
endif()


target_link_libraries(BfxApplication 
    PUBLIC Quickfix::Quickfix
    PUBLIC OpenSSL::SSL
    PUBLIC OpenSSL::Crypto
    PUBLIC Poco::Foundation
    )

target_include_directories(BfxApplication
    PUBLIC include
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src"
    PUBLIC Quickfix::Quickfix
    PUBLIC date::date
    PUBLIC OpenSSL::SSL
    PUBLIC OpenSSL::Crypto
    PUBLIC Poco::Foundation
    )

target_sources(BfxApplication
    PUBLIC src/BfxApplication.cpp
)   