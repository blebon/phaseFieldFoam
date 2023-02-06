cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

if(UNIX)
    message(STATUS "Determining id of the Linux distributor")
    execute_process(COMMAND lsb_release --short --id OUTPUT_VARIABLE LINUX_DISTRO OUTPUT_STRIP_TRAILING_WHITESPACE)
    # execute_process(COMMAND lsb_release --short --release OUTPUT_VARIABLE DISTRO_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
    if(LINUX_DISTRO MATCHES "CentOS")
        set(CMAKE_CXX_COMPILER /opt/wvec/OpenFOAM/ThirdParty-10/platforms/linux64/gcc-12.1.0/bin/c++)
    endif(LINUX_DISTRO MATCHES "CentOS")
endif(UNIX)

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
if(CMAKE_BUILD_TYPE MATCHES "Debug")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}gdb3 -DFULLDEBUG -O0 -gdwarf-4")
else()
    set(CMAKE_BUILD_TYPE Release)
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif(CMAKE_BUILD_TYPE MATCHES "Debug")
message(STATUS "Compiling in ${CMAKE_BUILD_TYPE} mode")
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

macro(build_OF_library)
    message(STATUS "Compiling and linking lib${OF_LIB_NAME}")
    add_library(${OF_LIB_NAME} SHARED ${OF_LIB_SOURCES})
    # Include dirs
    foreach(loop_include_path ${OF_INCLUDE_PATHS})
        target_include_directories(${OF_LIB_NAME} PUBLIC ${loop_include_path})
    endforeach()
    # Linking
    foreach(loop_lib ${OF_LINK_LIBRARIES})
        target_link_libraries(${OF_LIB_NAME} LINK_PUBLIC ${loop_lib})
    endforeach()
    target_link_libraries(${OF_LIB_NAME} LINK_PUBLIC OpenFOAM dl m)
    target_link_options(${OF_LIB_NAME} PUBLIC -fuse-ld=bfd)
    target_link_options(${OF_LIB_NAME} PUBLIC LINKER:--add-needed,--no-as-needed)
    # Copying to OpenFOAM lib path
    add_custom_command(TARGET ${OF_LIB_NAME}
                       COMMENT "Copying lib${OF_LIB_NAME}.so to $ENV{FOAM_USER_LIBBIN}"
                       POST_BUILD COMMAND cp lib${OF_LIB_NAME}.so $ENV{FOAM_USER_LIBBIN}/
    )
endmacro(build_OF_library)

macro(test_OF_library)
    message(STATUS "Testing lib${OF_LIB_NAME} in ${OF_TEST_DIR}/case")
    add_executable(${OF_TEST_NAME} ${OF_TEST_DIR}/${OF_TEST_NAME}.C)
    add_test(${OF_TEST_NAME} 
             COMMAND ${OF_TEST_NAME}
             WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/${OF_TEST_DIR}/case
    )
    # Include dirs
    foreach(loop_include_path ${OF_TEST_INCLUDE_PATHS})
        target_include_directories(${OF_TEST_NAME} PUBLIC ${loop_include_path})
    endforeach(loop_include_path ${OF_TEST_INCLUDE_PATHS})
    # Linking
    target_link_libraries(${OF_TEST_NAME} LINK_PUBLIC ${Boost_LIBRARIES})
    foreach(loop_lib ${OF_TEST_LINK_LIBRARIES})
        target_link_libraries(${OF_TEST_NAME} LINK_PUBLIC ${loop_lib})
    endforeach(loop_lib ${OF_TEST_LINK_LIBRARIES})
    target_link_libraries(${OF_TEST_NAME} LINK_PUBLIC OpenFOAM dl m)
    target_link_options(${OF_TEST_NAME} PUBLIC -fuse-ld=bfd)
    target_link_options(${OF_TEST_NAME} PUBLIC LINKER:--add-needed,--no-as-needed)
    add_custom_command(TARGET ${OF_TEST_NAME}
                       POST_BUILD 
                       COMMAND cp ${CMAKE_CURRENT_BINARY_DIR}/${OF_TEST_NAME} .
                       COMMAND blockMesh && ./${OF_TEST_NAME} --log_level=message
                       WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/${OF_TEST_DIR}/case                   
                       COMMENT "Running ${OF_TEST_NAME}"
    )
endmacro(test_OF_library)

set(OF_LIB_NAME myFvModels)
set(OF_LIB_SOURCES src/fvModels/derived/anisotropySource/anisotropySource.C)
set(OF_INCLUDE_PATHS "")
foreach(loop_include
    fvModels
)
    list(APPEND OF_INCLUDE_PATHS $ENV{FOAM_SRC}/${loop_include}/lnInclude)
endforeach()
set(OF_LINK_LIBRARIES "")
list(APPEND OF_LINK_LIBRARIES
    finiteVolume fvModels meshTools sampling
)
build_OF_library()

set(OF_TEST_NAME test_anisotropySource)
set(OF_TEST_DIR tests/fvModels)
set(OF_TEST_INCLUDE_PATHS ${OF_TEST_DIR})
set(OF_TEST_LINK_LIBRARIES)
list(APPEND OF_TEST_LINK_LIBRARIES 
    finiteVolume meshTools sampling myFvModels fvModels fvConstraints
)
test_OF_library()

# phaseFieldFoam
add_executable(${PROJECT_NAME} ${PHASEFIELDFOAM_DIR}/phaseFieldFoam.C)
# Linking
foreach(loop_lib
    finiteVolume myFvModels fvModels fvConstraints meshTools
)
    target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${loop_lib})
endforeach()
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC OpenFOAM dl m)
target_link_options(${PROJECT_NAME} PUBLIC -fuse-ld=bfd)
target_link_options(${PROJECT_NAME} PUBLIC LINKER:--add-needed,--no-as-needed)

# Copy phaseFieldFoam executable upon successful compilation
add_custom_command(TARGET ${PROJECT_NAME}
                   COMMENT "Copying ${PROJECT_NAME} to $ENV{FOAM_USER_APPBIN}"
                   POST_BUILD COMMAND cp ${PROJECT_NAME} $ENV{FOAM_USER_APPBIN}/
)