cmake_minimum_required(VERSION 3.9)

project(Vreeman2002 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif(MSVC)

# Boost Library
set(Boost_NO_SYSTEM_PATHS TRUE)
if (Boost_NO_SYSTEM_PATHS)
  set(BOOST_ROOT "/usr/include/boost169")
  set(BOOST_INCLUDE_DIR "/usr/include/boost169/boost")
  set(BOOST_LIBRARYDIR "/usr/lib64/boost169")
endif (Boost_NO_SYSTEM_PATHS)
find_package(Boost 1.69.0 COMPONENTS unit_test_framework REQUIRED)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

add_executable(test_phasefield test_phasefield.cpp)
add_test(NAME test_phasefield COMMAND test_phasefield)

TARGET_LINK_LIBRARIES( test_phasefield LINK_PUBLIC ${Boost_LIBRARIES} )