cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

project(phaseFieldFoam LANGUAGES CXX C)

message(STATUS "Using OpenFOAM $ENV{WM_PROJECT_VERSION}")

message(STATUS "Checking if required libraries are available")
message(STATUS "Finding OpenFOAM library")
find_library(OpenFOAM_LIB
    NAMES OpenFOAM libOpenFOAM
    PATHS ENV FOAM_LIBBIN
    REGISTRY_VIEW $ENV{WM_ARCH_OPTION}
    REQUIRED
)

message(STATUS "Finding dl library")
find_library(DL_LIB 
    NAMES dl libdl
    REGISTRY_VIEW $ENV{WM_ARCH_OPTION}
    REQUIRED
)

message(STATUS "Finding m library")
find_library(M_LIB
    NAMES m libm
    REGISTRY_VIEW $ENV{WM_ARCH_OPTION}
    REQUIRED
)

message(STATUS "Finding Boost library")
find_package(Boost 1.69.0 
    COMPONENTS unit_test_framework
)
if (NOT ${Boost_FOUND})
    if(UNIX)
        message(STATUS "Determining id of the Linux distributor")
        execute_process(COMMAND lsb_release --short --id OUTPUT_VARIABLE LINUX_DISTRO OUTPUT_STRIP_TRAILING_WHITESPACE)
        # execute_process(COMMAND lsb_release --short --release OUTPUT_VARIABLE DISTRO_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
        if (LINUX_DISTRO MATCHES "CentOS")
            message(STATUS "Using boost 1.69 on CentOS")
            set(Boost_NO_SYSTEM_PATHS TRUE)
            set(BOOST_ROOT "/usr/include/boost169")
            set(BOOST_INCLUDE_DIR "/usr/include/boost169/boost")
            set(BOOST_LIBRARYDIR "/usr/lib64/boost169")
        endif(LINUX_DISTRO MATCHES "CentOS")
    endif(UNIX)
    find_package(Boost 1.69.0
        REQUIRED
        COMPONENTS unit_test_framework
    )
endif(NOT ${Boost_FOUND})

enable_testing()

message(STATUS "Setting common OpenFOAM compilation options")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-m$ENV{WM_ARCH_OPTION})
add_compile_definitions($ENV{WM_ARCH})
add_compile_definitions(WM_ARCH_OPTION=$ENV{WM_ARCH_OPTION})
add_compile_definitions(WM_DP)
add_compile_definitions(WM_LABEL_SIZE=$ENV{WM_LABEL_SIZE})
add_compile_options(
    -Wall -Wextra -Wold-style-cast -Wnon-virtual-dtor -Wno-unused-parameter -Wno-invalid-offsetof -Wno-attributes
)
add_compile_options(-O3)
add_compile_definitions(NoRepository)
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftemplate-depth-100")
endif(CMAKE_COMPILER_IS_GNUCXX)
set(CMAKE_POSITION_INDEPENDENT_CODE ON) # -fPIC

message(STATUS "Setting common OpenFOAM include paths")
set(PHASEFIELDFOAM_DIR applications/solvers/multiphase/${PROJECT_NAME})

include_directories(${PHASEFIELDFOAM_DIR})
foreach(loop_include finiteVolume meshTools OpenFOAM OSspecific/POSIX)
    include_directories($ENV{FOAM_SRC}/${loop_include}/lnInclude)
endforeach(loop_include finiteVolume meshTools sampling dynamicFvMesh OpenFOAM OSspecific/POSIX)

message(STATUS "Setting common OpenFOAM link paths")
link_directories($ENV{FOAM_LIBBIN})
link_directories($ENV{FOAM_USER_LIBBIN})

# phaseFieldFoam
add_executable(${PROJECT_NAME} ${PHASEFIELDFOAM_DIR}/phaseFieldFoam.C)
# Linking
foreach(loop_lib
    finiteVolume fvModels fvConstraints meshTools
)
    target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${loop_lib})
endforeach()
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC OpenFOAM dl m)
target_link_options(${PROJECT_NAME} PUBLIC -fuse-ld=bfd)
target_link_options(${PROJECT_NAME} PUBLIC LINKER:--add-needed,--no-as-needed)

# Copy phaseFieldFoam executable upon successful compilation
add_custom_command(TARGET ${PROJECT_NAME}
                   COMMENT "Copying ${PROJECT_NAME} to $ENV{FOAM_USER_APPBIN}"
                   POST_BUILD COMMAND cp ${PROJECT_NAME} $ENV{FOAM_USER_APPBIN}
)