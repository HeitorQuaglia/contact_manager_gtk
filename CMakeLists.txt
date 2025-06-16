cmake_minimum_required(VERSION 3.31)
project(contact_manager C)

set(CMAKE_C_STANDARD 23)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk4)
include_directories(
    ${GTK_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
)

SET(SOURCES src/**/*.c)

add_executable(contact_manager src/main.c)

target_link_libraries(contact_manager ${GTK_LIBRARIES})
target_compile_options(contact_manager PRIVATE ${GTK_CFLAGS_OTHER})
