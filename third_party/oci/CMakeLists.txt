# Include this file only once
PROJECT(OCI)
if (USE_SYSTEM_OCI)
    find_package(OCI)
    if (NOT OCI_FOUND)
         MESSAGE(FATAL_ERROR "Cannot find system OCI library")
     endif()
endif()

if (NOT OCI_FOUND)
    message(STATUS "Using local OCI library")
    set(OCI_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)
    if(MSVC)
        if(SUBSYSTEM_NAME MATCHES "x86")
            set(OCI_LIBRARY_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/lib_generated/lib32)
            set(OCI_LIBRARY ${OCI_LIBRARY_DIRS}/oci32.lib)
        elseif(SUBSYSTEM_NAME MATCHES "x64")
            set(OCI_LIBRARY_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/lib_generated/lib64)
            set(OCI_LIBRARY ${OCI_LIBRARY_DIRS}/oci64.lib)
        else()
            message(FATAL_ERROR "Unknown SUBSYSTEM_NAME ${SUBSYSTEM_NAME}")
        endif()
    elseif(APPLE)
            set(OCI_LIBRARY_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/lib64)
        set(OCI_LIBRARY ${OCI_LIBRARY_DIRS}/liborasdk.dylib)
    elseif(UNIX)
    endif()
    set(OCI_FOUND YES)
endif()
