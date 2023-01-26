cmake_minimum_required(VERSION 3.17)

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
if(UNIX)
  message("-- Determining id of the Linux distributor")
  execute_process(COMMAND lsb_release --short --id OUTPUT_VARIABLE LINUX_DISTRO OUTPUT_STRIP_TRAILING_WHITESPACE)
  # execute_process(COMMAND lsb_release --short --release OUTPUT_VARIABLE DISTRO_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
  if (LINUX_DISTRO MATCHES "CentOS") # If on davy or v-sys-bcast, distro will be Centos
    message("-- Using boost 1.69 on CentOS")
    set(Boost_NO_SYSTEM_PATHS TRUE)
    set(BOOST_ROOT "/usr/include/boost169")
    set(BOOST_INCLUDE_DIR "/usr/include/boost169/boost")
    set(BOOST_LIBRARYDIR "/usr/lib64/boost169")
  endif(LINUX_DISTRO MATCHES "CentOS")
endif(UNIX)
find_package(Boost 1.69.0 COMPONENTS unit_test_framework REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

# Compile and link test executable
add_executable(test_phasefield test_phasefield.cpp)
add_test(NAME test_phasefield COMMAND test_phasefield)
target_link_libraries(test_phasefield LINK_PUBLIC ${Boost_LIBRARIES})

# Run tests after making
add_custom_command(TARGET test_phasefield
                   COMMENT "Run tests"
                   POST_BUILD COMMAND test_phasefield --color_output=yes --log_level=message
)