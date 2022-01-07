find_package(GTest REQUIRED)

add_executable(CliClient CliClient.cpp)

target_link_libraries(CliClient PUBLIC Bfxclient)

target_include_directories(CliClient PUBLIC Bfxclient)