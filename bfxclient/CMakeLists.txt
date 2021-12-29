find_package(Quickfix REQUIRED)
find_package(OpenSSL REQUIRED)

add_library(Bfxclient BfxClient.cpp)

if(OPENSSL_FOUND)
target_compile_definitions(Bfxclient PRIVATE HAVE_SSL=1)
endif()

target_link_libraries(Bfxclient 
    PUBLIC Quickfix::Quickfix
    PRIVATE OpenSSL::SSL
    PRIVATE OpenSSL::Crypto)

target_include_directories(Bfxclient
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src"
    PUBLIC Quickfix::Quickfix
    PRIVATE OpenSSL::SSL
    PRIVATE OpenSSL::Crypto
    )

target_sources(Bfxclient
    PRIVATE src/BfxApplication.cpp
)   

add_subdirectory(test)