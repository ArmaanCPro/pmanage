cmake_minimum_required(VERSION 3.30)

project (pmanageTests CXX)

add_executable(pmanageTests)

target_sources(pmanageTests
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/tests.cpp"
)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/9156d4caac880b513264ecbe0aa4746a3fead3d7.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

target_link_libraries(pmanageTests
    PRIVATE pmanage::common pmanage::pmanage GTest::gtest_main GTest::gtest
)
include(GoogleTest)
gtest_discover_tests(pmanageTests)

enable_sanitizers(pmanageTests)
enable_ipo(pmanageTests)

