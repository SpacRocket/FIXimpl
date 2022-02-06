find_package(GTest REQUIRED)

add_executable(Cli Cli.cpp)

target_link_libraries(Cli PUBLIC Bfxclient)

target_include_directories(Cli PUBLIC Bfxclient)