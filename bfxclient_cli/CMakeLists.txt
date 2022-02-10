cmake_minimum_required(VERSION 3.22)
include_guard(GLOBAL)
set(CMAKE_CXX_STANDARD 20)
project(BfxClientCli)

find_package(Quickfix REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(date REQUIRED)
find_package(PostgreSQL REQUIRED)

add_executable(BfxClientCli src/BfxClientCli.cpp)

add_subdirectory(../bfxclient_abstract sub/build_client)
add_subdirectory(../bfxapplication sub/build_application)

if(OPENSSL_FOUND)
target_compile_definitions(BfxClientCli PUBLIC HAVE_SSL=1)
endif()

if(PostgreSQL_FOUND)
target_compile_definitions(BfxClientCli PUBLIC HAVE_POSTGRESQL=1)
endif()

target_link_libraries(BfxClientCli 
    PUBLIC Quickfix::Quickfix
    PUBLIC PostgreSQL::PostgreSQL
    PUBLIC OpenSSL::SSL
    PUBLIC OpenSSL::Crypto
    PUBLIC date::date
    PUBLIC BfxApplication
    PRIVATE BfxClientAbstract
    )

target_include_directories(BfxClientCli
    PUBLIC include
    PUBLIC Quickfix::Quickfix
    PUBLIC PostgreSQL::PostgreSQL
    PUBLIC date::date
    PUBLIC OpenSSL::SSL
    PUBLIC OpenSSL::Crypto
    PUBLIC BfxApplication
    PRIVATE BfxClientAbstract
    )

target_sources(BfxClientCli PRIVATE src/BfxApplicationCLI.cpp)