# The Flutter tooling requires that developers have CMake 3.10 or later
# installed. You should not increase this version, as doing so will cause
# the plugin to fail to compile for some customers of the plugin.
cmake_minimum_required(VERSION 3.10)

project(flutter_lame_library VERSION 0.0.1 LANGUAGES C)

# Configure 16KB page size support
if(ANDROID)
    if(ANDROID_ABI STREQUAL "arm64-v8a" OR ANDROID_ABI STREQUAL "x86_64")
        add_definitions(-DPAGE_SIZE=16384)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DPAGE_SIZE=16384")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPAGE_SIZE=16384")
    endif()
endif()

add_subdirectory(lame)
target_compile_definitions(mp3lame PUBLIC DART_SHARED_LIB)

# Set page size for specific architectures
if(ANDROID_ABI STREQUAL "arm64-v8a")
    set_target_properties(mp3lame PROPERTIES OUTPUT_NAME "mp3lame-arm64-v8a")
elseif(ANDROID_ABI STREQUAL "x86_64")
    set_target_properties(mp3lame PROPERTIES OUTPUT_NAME "mp3lame-x86_64")
endif()
