#
# Automatically generated file; DO NOT EDIT.
# CMake toolchain file for Buildroot
#

# In order to allow the toolchain to be relocated, we calculate the
# HOST_DIR based on this file's location: $(HOST_DIR)/share/buildroot
# and store it in RELOCATED_HOST_DIR.
# All the other variables that need to refer to HOST_DIR will use the
# RELOCATED_HOST_DIR variable.
string(REPLACE "/share/buildroot" "" RELOCATED_HOST_DIR ${CMAKE_CURRENT_LIST_DIR})

# Point cmake to the location where we have our custom modules,
# so that it can find our custom platform description.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

set(CMAKE_SYSTEM_NAME Buildroot)
set(CMAKE_SYSTEM_PROCESSOR armv7l)

# Set the {C,CXX}FLAGS appended by CMake depending on the build type
# defined by Buildroot. CMake defaults these variables with -g and/or
# -O options, and they are appended at the end of the argument list,
# so the Buildroot options are overridden. Therefore these variables
# have to be cleared, so that the options passed in CMAKE_C_FLAGS do
# apply.
#
# Note:
#   if the project forces some of these flag variables, Buildroot is
#   screwed up and there is nothing Buildroot can do about that :(
set(CMAKE_C_FLAGS_DEBUG "" CACHE STRING "Debug CFLAGS")
set(CMAKE_CXX_FLAGS_DEBUG "" CACHE STRING "Debug CXXFLAGS")
set(CMAKE_C_FLAGS_RELEASE " -DNDEBUG" CACHE STRING "Release CFLAGS")
set(CMAKE_CXX_FLAGS_RELEASE " -DNDEBUG" CACHE STRING "Release CXXFLAGS")

# Build type from the Buildroot configuration
set(CMAKE_BUILD_TYPE Release CACHE STRING "Buildroot build configuration")

# Buildroot defaults flags.
# If you are using this toolchainfile.cmake file outside of Buildroot and
# want to customize the compiler/linker flags, then:
# * set them all on the cmake command line, e.g.:
#     cmake -DCMAKE_C_FLAGS="-D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -Os -Dsome_custom_flag" ...
# * and make sure the project's CMake code extends them like this if needed:
#     set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Dsome_definitions")
set(CMAKE_C_FLAGS "-D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -Os" CACHE STRING "Buildroot CFLAGS")
set(CMAKE_CXX_FLAGS "-D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -Os" CACHE STRING "Buildroot CXXFLAGS")
set(CMAKE_EXE_LINKER_FLAGS "" CACHE STRING "Buildroot LDFLAGS for executables")
set(CMAKE_SHARED_LINKER_FLAGS "" CACHE STRING "Buildroot LDFLAGS for shared libraries")
set(CMAKE_MODULE_LINKER_FLAGS "" CACHE STRING "Buildroot LDFLAGS for module libraries")

set(CMAKE_INSTALL_SO_NO_EXE 0)

set(CMAKE_PROGRAM_PATH "${RELOCATED_HOST_DIR}/bin")
set(CMAKE_SYSROOT "${RELOCATED_HOST_DIR}/arm-buildroot-linux-gnueabihf/sysroot")
set(CMAKE_FIND_ROOT_PATH "${RELOCATED_HOST_DIR}/arm-buildroot-linux-gnueabihf/sysroot")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(ENV{PKG_CONFIG_SYSROOT_DIR} "${RELOCATED_HOST_DIR}/arm-buildroot-linux-gnueabihf/sysroot")

# This toolchain file can be used both inside and outside Buildroot.
set(CMAKE_C_COMPILER "${RELOCATED_HOST_DIR}/bin/arm-buildroot-linux-gnueabihf-gcc")
set(CMAKE_CXX_COMPILER "${RELOCATED_HOST_DIR}/bin/arm-buildroot-linux-gnueabihf-g++")
if(0)
  set(CMAKE_Fortran_FLAGS_DEBUG "" CACHE STRING "Debug Fortran FLAGS")
  set(CMAKE_Fortran_FLAGS_RELEASE " -DNDEBUG" CACHE STRING "Release Fortran FLAGS")
  set(CMAKE_Fortran_FLAGS "-Os" CACHE STRING "Buildroot FCFLAGS")
  set(CMAKE_Fortran_COMPILER "${RELOCATED_HOST_DIR}/bin/arm-buildroot-linux-gnueabihf-gfortran")
endif()
