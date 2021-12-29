find_package(GTest REQUIRED)
find_library(Bfxclient ./.. REQUIRED)

enable_testing()

add_executable(Bfxclient_UnitTest AuthenticationTest.cpp)

target_link_libraries(Bfxclient_UnitTest PUBLIC GTest::Main
                                         PUBLIC Bfxclient)

target_include_directories(Bfxclient_UnitTest PUBLIC Bfxclient
                                              PUBLIC GTest::GTest)