find_package(GTest REQUIRED)

enable_testing()

add_executable(Bfxclient_UnitTest BfxClientTests.cpp)

target_link_libraries(Bfxclient_UnitTest PUBLIC GTest::Main
                                         PUBLIC BfxClientAbstract)

target_include_directories(Bfxclient_UnitTest 
PUBLIC BfxClientAbstract
PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
PUBLIC GTest::GTest)