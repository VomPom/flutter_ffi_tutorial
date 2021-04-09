cmake_minimum_required(VERSION 3.4.1)  # for example
# find_library( # Defines the name of the path variable that stores the
#               # location of the NDK library.
#               log-lib
#               # Specifies the name of the NDK library that
#               # CMake needs to locate.
#               log )

add_library(ffi_invoke
             # Sets the library as a shared library.
        SHARED
             # Provides a relative path to your source file(s).
        ffi_invoke.cpp
        jni_helper.cpp
        ffi_async_utils.cpp
        internal/dart_api_dl.c)

target_link_libraries(
        ffi_invoke
        log )