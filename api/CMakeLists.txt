cmake_minimum_required(VERSION 3.4)

project(Indigo)

include(indigo-version.cmake)
include(DefineTest)

MESSAGE(STATUS "Indigo version is ${INDIGO_VERSION}")
SET(INDIGO_VERSION "${INDIGO_VERSION}" PARENT_SCOPE) # promote Indigo version
configure_file(${Indigo_SOURCE_DIR}/src/indigo_version.h.in ${Indigo_SOURCE_DIR}/src/indigo_version.h)

file (GLOB Indigo_src src/*.c*)
file (GLOB Indigo_headers *.h* src/*.h*)

# Get defined variable with the headers for TinyXML
get_directory_property(TinyXML_HEADERS_DIR DIRECTORY ../third_party/tinyxml DEFINITION TinyXML_HEADERS_DIR)

include_directories(${Indigo_SOURCE_DIR} ${Common_SOURCE_DIR} ${Common_SOURCE_DIR}/.. ${TinyXML_HEADERS_DIR} ${InChI_SOURCE_DIR}/inchi_dll)

add_library(indigoObj OBJECT ${Indigo_src} ${Indigo_headers})
set_target_properties(indigoObj PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS}")

#Indigo static
if (WITH_STATIC)
    add_library(indigo STATIC $<TARGET_OBJECTS:indigoObj> $<TARGET_OBJECTS:graph> $<TARGET_OBJECTS:layout> $<TARGET_OBJECTS:molecule> $<TARGET_OBJECTS:common> $<TARGET_OBJECTS:reaction>)
    SET_TARGET_PROPERTIES(indigo PROPERTIES OUTPUT_NAME "indigo-static")
    target_link_libraries(indigo tinyxml z inchi)
    set_property(TARGET indigo PROPERTY FOLDER "indigo")
    # No exports in case of static library: define empty EXPORT_SYMBOL definition
    set_target_properties(indigo PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS} -DEXPORT_SYMBOL= ")
    pack_static(indigo)
endif()


# Indigo shared
add_library(indigo-shared SHARED $<TARGET_OBJECTS:indigoObj> $<TARGET_OBJECTS:graph> $<TARGET_OBJECTS:layout> $<TARGET_OBJECTS:molecule> $<TARGET_OBJECTS:common> $<TARGET_OBJECTS:reaction> ${Common_SOURCE_DIR}/hacks/memcpy.c)

# Copy shared library to indigo/api/libs/shared/ folder, that is used by python tests and wrappers generation
add_custom_command(
        TARGET indigo-shared
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/libs/shared/${SYSTEM_NAME}/${SUBSYSTEM_NAME}
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:indigo-shared> ${CMAKE_CURRENT_SOURCE_DIR}/libs/shared/${SYSTEM_NAME}/${SUBSYSTEM_NAME}/)

SET_TARGET_PROPERTIES(indigo-shared PROPERTIES OUTPUT_NAME "indigo")
set_target_properties(indigo-shared PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS}")
if (APPLE)
    set_target_properties(indigo-shared PROPERTIES LINK_FLAGS "${LINK_FLAGS} -undefined dynamic_lookup")
endif()
if (UNIX AND NOT APPLE)
    if(${SUBSYSTEM_NAME} MATCHES "x64")
        set_target_properties(indigo-shared PROPERTIES LINK_FLAGS "${LINK_FLAGS} -Wl,--wrap=memcpy")
    endif()
    target_link_libraries(indigo-shared -lpthread -lrt)
endif()
if (MSVC OR MINGW)
    set_target_properties(indigo-shared PROPERTIES PREFIX "")
endif()
target_link_libraries(indigo-shared z tinyxml inchi)
if(UNIX AND NOT APPLE)
    set_property(TARGET indigo-shared PROPERTY LINK_INTERFACE_LIBRARIES "-lpthread;-lrt;z;tinyxml;inchi")
else()
    set_property(TARGET indigo-shared PROPERTY LINK_INTERFACE_LIBRARIES "z;tinyxml;inchi")
endif()
set_property(TARGET indigo-shared PROPERTY FOLDER "indigo")
if (NOT PACK_INDIGO_NOT)
    pack_shared(indigo-shared)
endif()
if (NOT DEFINED ENV{DISABLE_INDIGO_TESTS})
DEFINE_TEST(indigo-c-test-shared "tests/c/indigo-test.c" indigo-shared)
endif()

add_executable(dlopen-test ${Indigo_SOURCE_DIR}/tests/c/dlopen-test.c)
if (UNIX AND NOT APPLE)
    set_target_properties(dlopen-test PROPERTIES LINK_FLAGS "-rdynamic -ldl -pthread")
    if (NOT APPLE AND ${SUBSYSTEM_NAME} MATCHES "x64")
        set_target_properties(dlopen-test PROPERTIES LINK_FLAGS "-rdynamic -ldl -pthread -Wl,--wrap=memcpy")
    endif()
    TARGET_LINK_LIBRARIES(dlopen-test "-rdynamic -ldl -lpthread")
endif()
set_property(TARGET dlopen-test PROPERTY FOLDER "tests")
