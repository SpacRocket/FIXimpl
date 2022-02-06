find_package(Quickfix REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(date REQUIRED)
find_package(PostgreSQL REQUIRED)

add_library(Bfxclient INTERFACE BfxClient.hpp)

if(OPENSSL_FOUND)
target_compile_definitions(Bfxclient INTERFACE HAVE_SSL=1)
endif()

if(PostgreSQL_FOUND)
target_compile_definitions(Bfxclient INTERFACE HAVE_POSTGRESQL=1)
endif()

target_link_libraries(Bfxclient 
    INTERFACE Quickfix::Quickfix
    INTERFACE PostgreSQL::PostgreSQL
    INTERFACE OpenSSL::SSL
    INTERFACE OpenSSL::Crypto
    INTERFACE date::date
    )

target_include_directories(Bfxclient
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}"
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/src"    
    INTERFACE Quickfix::Quickfix
    INTERFACE PostgreSQL::PostgreSQL
    INTERFACE date::date
    INTERFACE OpenSSL::SSL
    INTERFACE OpenSSL::Crypto
    )

target_sources(Bfxclient
    INTERFACE src/BfxApplication.cpp
)   

add_subdirectory(test)
add_subdirectory(interfaces/cli)
add_subdirectory(interfaces/qRPC)