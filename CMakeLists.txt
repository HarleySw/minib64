cmake_minimum_required(VERSION 3.16)

project(minib64
  VERSION 1.0.0
  DESCRIPTION "A minimal Base64 encoder/decoder in C"
  LANGUAGES C
)

# Set CMake to export compile commands for tools like clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(minib64
  STATIC src/base64.c
)

# Set the include directories for the minib64 target
# Build interface includes the source directory's include folder, while the install interface includes the install directory's include folder
target_include_directories(minib64
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> # Include directory for the build interface
    $<INSTALL_INTERFACE:include> # Include directory for the install interface
)

# Set the C standard to C11 for the minib64 target
target_compile_features(minib64
  PUBLIC c_std_11
)

# Add the examples subdirectory to the build
add_subdirectory(examples/)

# Install rules
include(GNUInstallDirs)

# Install the minib64 library to the appropriate directories
install(
  TARGETS minib64
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

#Install the header files to the appropriate include directory
install(
  DIRECTORY include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Enable testing and add the tests subdirectory if BUILD_TESTING is ON
include(CTest)
if(BUILD_TESTING)
  add_subdirectory(external/unity/)
  add_subdirectory(tests/)
endif()