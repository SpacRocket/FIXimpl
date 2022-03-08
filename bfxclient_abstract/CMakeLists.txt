cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
INCLUDE_GUARD(GLOBAL)
project(BfxClientAbstract)
set(CMAKE_CXX_STANDARD 20)

find_package(Quickfix REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(date REQUIRED)

add_library(BfxClientAbstract INTERFACE include/BfxClientAbstract.hpp)

add_subdirectory(../bfxapplication build)

if(OPENSSL_FOUND)
target_compile_definitions(BfxClientAbstract INTERFACE HAVE_SSL=1)
endif()

target_link_libraries(BfxClientAbstract 
    INTERFACE Quickfix::Quickfix
    INTERFACE OpenSSL::SSL
    INTERFACE OpenSSL::Crypto
    INTERFACE date::date
    INTERFACE BfxApplication
    )

target_include_directories(BfxClientAbstract
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}"
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/src"    
    INTERFACE Quickfix::Quickfix
    INTERFACE date::date
    INTERFACE OpenSSL::SSL
    INTERFACE OpenSSL::Crypto
    INTERFACE BfxApplication
    )
    
add_subdirectory(test)