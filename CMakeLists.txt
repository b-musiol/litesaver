cmake_minimum_required(VERSION 3.14)

project(litesaver)

add_library(litesaver
  src/Litesaver.cpp
  src/Core.cpp
  src/queries.cpp
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

target_compile_features(litesaver PUBLIC cxx_std_20)


include(FetchContent)
FetchContent_Declare(
  SQLiteDB
  GIT_REPOSITORY https://github.com/b-musiol/SQLiteDB.git
  GIT_TAG        main
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(SQLiteDB)

target_include_directories(litesaver PUBLIC
    ${sqlitedb_SOURCE_DIR}/incl/
)

target_link_libraries(litesaver PRIVATE SQLiteDB)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.17.0  # Or use a specific commit hash
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(RunTests_Litesaver test/Litesaver_test.cpp)
target_link_libraries(RunTests_Litesaver litesaver GTest::gtest_main)
target_compile_features(RunTests_Litesaver PUBLIC cxx_std_20)


include(GoogleTest)
gtest_discover_tests(RunTests_Litesaver)

enable_testing()
