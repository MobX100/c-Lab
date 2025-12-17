cmake_minimum_required(VERSION 3.14)
project(lab5 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ===========================
# Fetch Catch2
# ===========================
include(FetchContent)

# --- Catch2 ---
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.7.1  # latest as of 2025
)
FetchContent_MakeAvailable(Catch2)

# --- Boost (header-only) ---
# We only need boost/variant/recursive_wrapper.hpp and its dependencies.
# Fetch the entire boost superproject and include its root as include directory.
FetchContent_Declare(
  Boost
  GIT_REPOSITORY https://github.com/boostorg/boost.git
  GIT_TAG boost-1.83.0
)
FetchContent_GetProperties(Boost)
if(NOT boost_POPULATED)
  FetchContent_Populate(Boost)
  # Bootstrap: run bootstrap.sh or bootstrap.bat to generate headers
  if(CMAKE_HOST_UNIX)
    execute_process(
      COMMAND ./bootstrap.sh --with-libraries=none
      WORKING_DIRECTORY ${boost_SOURCE_DIR}
      RESULT_VARIABLE BOOST_BOOTSTRAP_RESULT
    )
    if(NOT BOOST_BOOTSTRAP_RESULT EQUAL 0)
      message(FATAL_ERROR "Failed to bootstrap Boost")
    endif()
    execute_process(
      COMMAND ./b2 headers
      WORKING_DIRECTORY ${boost_SOURCE_DIR}
      RESULT_VARIABLE BOOST_HEADERS_RESULT
    )
    if(NOT BOOST_HEADERS_RESULT EQUAL 0)
      message(FATAL_ERROR "Failed to generate Boost headers")
    endif()
  else()
    # Windows
    execute_process(
      COMMAND cmd /c bootstrap.bat
      WORKING_DIRECTORY ${boost_SOURCE_DIR}
      RESULT_VARIABLE BOOST_BOOTSTRAP_RESULT
    )
    if(NOT BOOST_BOOTSTRAP_RESULT EQUAL 0)
      message(FATAL_ERROR "Failed to bootstrap Boost (Windows)")
    endif()
    execute_process(
      COMMAND b2 headers
      WORKING_DIRECTORY ${boost_SOURCE_DIR}
      RESULT_VARIABLE BOOST_HEADERS_RESULT
    )
    if(NOT BOOST_HEADERS_RESULT EQUAL 0)
      message(FATAL_ERROR "Failed to generate Boost headers (Windows)")
    endif()
  endif()
endif()

# ===========================
# Build
# ===========================
add_executable(lab5_tests variant.cpp)

# Include directories
target_include_directories(lab5_tests PRIVATE
  ${boost_SOURCE_DIR}
  ${catch2_SOURCE_DIR}/single_include
)

target_link_libraries(lab5_tests Catch2::Catch2WithMain)

# Enable testing
enable_testing()
add_test(NAME lab5 COMMAND lab5_tests)