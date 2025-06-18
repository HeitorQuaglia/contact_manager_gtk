cmake_minimum_required(VERSION 3.16)
project(contact_manager C)

set(CMAKE_C_STANDARD 23)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk4)
pkg_check_modules(ADWAITA REQUIRED libadwaita-1)
find_package(Intl REQUIRED)

find_library(UUID_LIBRARY uuid)
if (NOT UUID_LIBRARY)
    message(FATAL_ERROR "libuuid not found")
endif()

# GResource compilation
find_program(GLIB_COMPILE_RESOURCES glib-compile-resources)
if (NOT GLIB_COMPILE_RESOURCES)
    message(FATAL_ERROR "glib-compile-resources not found")
endif()

set(gresource_output ${CMAKE_CURRENT_BINARY_DIR}/resources.c)
set(RESOURCES_XML ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/resources.xml)

set(UI_FILES
    src/ui/main_window.ui
    src/ui/phonebooks_view.ui
    src/ui/contacts_view.ui
)

add_custom_command(
    OUTPUT ${gresource_output}
    COMMAND ${GLIB_COMPILE_RESOURCES} --target=${gresource_output} --sourcedir=${CMAKE_CURRENT_SOURCE_DIR}/src/ui --generate-source ${RESOURCES_XML}
    DEPENDS ${RESOURCES_XML}
            ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/main_window.ui
            ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/phonebooks_view.ui
            ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/contacts_view.ui
)

add_executable(contact_manager
    src/main.c
    src/main_window.c
    src/util.c
    src/phonebook.c
    src/phonebook_object.c
    src/contact.c
    src/contact_object.c
    src/phone.c
    src/phone_object.c
    src/address.c
    src/address_object.c
    src/phonebook_manager.c
    src/ui/phonebooks_view.c
    src/ui/contacts_view.c
    ${gresource_output}
)

target_include_directories(contact_manager PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GTK_INCLUDE_DIRS}
    ${ADWAITA_INCLUDE_DIRS}
)

target_link_libraries(contact_manager PRIVATE
    ${GTK_LIBRARIES}
    ${ADWAITA_LIBRARIES}
    ${UUID_LIBRARY}
    ${Intl_LIBRARIES}
)

target_compile_options(contact_manager PRIVATE ${GTK_CFLAGS_OTHER})
