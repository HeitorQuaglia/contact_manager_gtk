cmake_minimum_required(VERSION 3.16)
project(contact_manager C)

set(CMAKE_C_STANDARD 23)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk4)

find_library(UUID_LIBRARY uuid)
if (NOT UUID_LIBRARY)
    message(FATAL_ERROR "libuuid not found")
endif()

set(SOURCES
        src/main.c
        src/util.c
        src/phonebook.c
        src/contact.c
        src/phone.c
        src/address.c
        src/ui/main_window.c
)

add_executable(contact_manager ${SOURCES})

target_include_directories(contact_manager PRIVATE
        ${GTK_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/ui
)

target_link_libraries(contact_manager
        ${GTK_LIBRARIES}
        ${UUID_LIBRARY}
)

target_compile_options(contact_manager PRIVATE ${GTK_CFLAGS_OTHER})
