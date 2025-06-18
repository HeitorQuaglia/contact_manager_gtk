cmake_minimum_required(VERSION 3.16)
project(contact_manager C)

set(CMAKE_C_STANDARD 23)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk4)

find_library(UUID_LIBRARY uuid)
if (NOT UUID_LIBRARY)
    message(FATAL_ERROR "libuuid not found")
endif()

# GResource compilation
find_program(GLIB_COMPILE_RESOURCES glib-compile-resources)
if (NOT GLIB_COMPILE_RESOURCES)
    message(FATAL_ERROR "glib-compile-resources not found")
endif()

set(RESOURCES_XML ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/resources.xml)
set(RESOURCES_C ${CMAKE_CURRENT_BINARY_DIR}/resources.c)
add_custom_command(
    OUTPUT ${RESOURCES_C}
    COMMAND ${GLIB_COMPILE_RESOURCES} --target=${RESOURCES_C} --sourcedir=${CMAKE_CURRENT_SOURCE_DIR}/src/ui --generate-source ${RESOURCES_XML}
    DEPENDS ${RESOURCES_XML}
)

set(SOURCES
        src/main.c
        src/util.c
        src/phonebook.c
        src/contact.c
        src/contact_object.c
        src/phone.c
        src/phone_object.c
        src/address.c
        src/address_object.c
        src/phonebook_manager.c
        src/ui/phonebook_manager_window.c
        src/ui/contact_manager_window.c
        src/ui/contact_detail_window.c
        ${RESOURCES_C}
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
