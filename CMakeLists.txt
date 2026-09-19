cmake_minimum_required(VERSION 3.16)

project(minib64
  VERSION 1.0.0
  DESCRIPTION "A minimal Base64 encoder/decoder in C"
  LANGUAGES C
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(minib64
  STATIC src/base64.c
)

target_include_directories(minib64
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(minib64
  PUBLIC c_std_11
)

add_subdirectory(examples/)

include(GNUInstallDirs)

install(
  TARGETS minib64
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
  DIRECTORY include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
