cmake_minimum_required(VERSION 3.21)
include_guard(GLOBAL)
set(CMAKE_CXX_STANDARD 20)
project(BfxClientQRpc)

find_package(Quickfix REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(date REQUIRED)
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)

add_executable(BfxClientQRpc)

target_link_libraries(BfxClientQRpc 
    PUBLIC Quickfix::Quickfix
    PUBLIC OpenSSL::SSL
    PUBLIC OpenSSL::Crypto
    PUBLIC date::date
    PUBLIC BfxApplication
    PRIVATE BfxClientAbstract
    )

target_include_directories(BfxClientQRpc
    PUBLIC include
    PUBLIC Quickfix::Quickfix
    PUBLIC date::date
    PUBLIC OpenSSL::SSL
    PUBLIC OpenSSL::Crypto
    PUBLIC BfxApplication
    PRIVATE BfxClientAbstract
    )

target_sources(BfxClientQRpc PRIVATE "./src/BfxApplicaitonQRpc.cpp")