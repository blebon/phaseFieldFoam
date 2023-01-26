cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

project(phaseFieldFoam LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compilation options
add_definitions("-m$ENV{WM_ARCH_OPTION} -D$ENV{WM_ARCH} -DWM_ARCH_OPTION=$ENV{WM_ARCH_OPTION} -DWM_DP -DWM_LABEL_SIZE=$ENV{WM_LABEL_SIZE} -Wall -Wextra -Wold-style-cast -Wnon-virtual-dtor -Wno-unused-parameter -Wno-invalid-offsetof -Wno-attributes -O3  -DNoRepository -ftemplate-depth-100 -fPIC")

# Check if OpenFOAM is present
MESSAGE(STATUS "OpenFOAM $ENV{WM_PROJECT_VERSION} found")

# Match path configuration of Make/options
set(PHASEFIELDFOAM_DIR applications/solvers/multiphase/phaseFieldFoam)

include_directories($ENV{FOAM_SRC}/finiteVolume/lnInclude)
include_directories($ENV{FOAM_SRC}/meshTools/lnInclude)
include_directories(${PHASEFIELDFOAM_DIR})
include_directories($ENV{FOAM_SRC}/OpenFOAM/lnInclude)
include_directories($ENV{FOAM_SRC}/OSspecific/POSIX/lnInclude)

link_directories($ENV{FOAM_LIBBIN})

# Add executable
add_executable(${PROJECT_NAME} ${PHASEFIELDFOAM_DIR}/phaseFieldFoam.C)

# Link libraries
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC finiteVolume fvModels fvConstraints meshTools OpenFOAM dl m)
target_link_options(${PROJECT_NAME} PUBLIC -fuse-ld=bfd)
target_link_options(${PROJECT_NAME} PUBLIC LINKER:--add-needed,--no-as-needed)

# Copy phaseFieldFoam executable upon successful compilation
add_custom_command(TARGET ${PROJECT_NAME}
                   COMMENT "Copying ${PROJECT_NAME} to $ENV{FOAM_USER_APPBIN}"
                   POST_BUILD COMMAND cp ${PROJECT_NAME} $ENV{FOAM_USER_APPBIN}
)