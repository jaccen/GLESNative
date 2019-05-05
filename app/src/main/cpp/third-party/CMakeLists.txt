#cmake_minimum_required(VERSION 3.6.0)
#
#project(AREngine)
#
#set(TARGET_NAME AREngine)
#
#set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/)
#set(LOCAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/.local)
#set(LOCAL_GENERATED_DIR ${LOCAL_DIR}/generated)
#set(ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets)
#set(SCRIPT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/scripts)
#set(APP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/app)
#
#set(THIRDPARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third-party)
#
#if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
#    message("Generate AREngine for IOS...")
#    ADD_DEFINITIONS(-DDUAR_IOS)
#
#    set(IOS TRUE)
#    set(APPLE TRUE)
#    set(PLATFORM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/platforms/iOS)
#    set(LOCAL_ENGINE_BUILD_DIR ${LOCAL_DIR}/build/AREngine/IOS)
#    set(THIRDPARTY_LIB_DIR ${THIRDPARTY_DIR}/lib/ios)
#elseif(${CMAKE_SYSTEM_NAME} MATCHES "Android")
#    message("Generate AREngine for Android...")
#    ADD_DEFINITIONS(-DDUAR_ANDROID)
#    ADD_DEFINITIONS(-DANDROID)
#    ADD_DEFINITIONS(-D__ANDROID__)
#
#    set(PLATFORM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/platforms/Android)
#    set(LOCAL_ENGINE_BUILD_DIR ${LOCAL_DIR}/build/AREngine/Android)
#    set(THIRDPARTY_LIB_DIR ${THIRDPARTY_DIR}/lib/android)
#    set(ANDROID TRUE)
#
#    if(${CMAKE_HOST_WIN32})
#	    set(ANDROID_LIB_PATH  $ENV{AppData}/../Local)
#    else()
#	    set(ANDROID_LIB_PATH  $ENV{HOME}/Library)
#    endif()
#
#    if(${ANDROID_ABI} STREQUAL "armeabi-v7a")
#        set(ANDROID_TOOLCHAIN_INC ${ANDROID_LIB_PATH}/Android/sdk/ndk-bundle/sysroot/usr/include/arm-linux-androideabi)
#    elseif(${ANDROID_ABI} STREQUAL "arm64-v8a")
#        set(ANDROID_TOOLCHAIN_INC ${ANDROID_LIB_PATH}/Android/sdk/ndk-bundle/sysroot/usr/include/aarch64-linux-android)
#    endif()
#endif()
#
#set(LOCAL_BIN_DIR ${LOCAL_ENGINE_BUILD_DIR}/bin)
#set(LOCAL_LIB_DIR ${LOCAL_ENGINE_BUILD_DIR}/lib)
#
#if(NOT ANDROID)
#    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${LOCAL_BIN_DIR})
#    set(EXECUTABLE_OUTPUT_PATH         ${LOCAL_BIN_DIR})
#    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LOCAL_LIB_DIR})
#    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LOCAL_LIB_DIR})
#    set(CMAKE_BINARY_DIR ${LOCAL_BIN_DIR})
#    LINK_DIRECTORIES(${LOCAL_LIB_DIR})
#endif()
#
#add_definitions(-D__STDC_LIMIT_MACROS)
#add_definitions(-D__STDC_FORMAT_MACROS)
#add_definitions(-D__STDC_CONSTANT_MACROS)
#
#if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
#    ADD_DEFINITIONS(-D_DEBUG)
#    ADD_DEFINITIONS(-DDUAR_DEBUG)
#    # TODO: -fnortti
#    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -MMD -MP -MP -Wall -Wextra -fomit-frame-pointer -g -m64 -fno-exceptions")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -MMD -MP -MP  -fomit-frame-pointer -g -m64 -fno-exceptions")
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -MMD -MP -MP -Wall -Wextra -fomit-frame-pointer -g -m64")
#elseif(${CMAKE_BUILD_TYPE} MATCHES "Release")
#    ADD_DEFINITIONS(-DNDEBUG)
#    ADD_DEFINITIONS(-DDUAR_RELEASE)
#    # TODO: -fnortti
#    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -MMD -MP -MP -Wall -Wextra -fomit-frame-pointer -g -O3 -m64 -fno-exceptions")
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -MMD -MP -MP -fomit-frame-pointer -g -O3 -m64")
#endif()
#
##include(${THIRDPARTY_DIR}/bgfx/CMakeLists.txt)
#add_subdirectory(src/engine)
#
#if(${CMAKE_TOOLCHAIN_FILE} MATCHES "NO WARNING")
#    message("NO WARNING")
#endif()


cmake_minimum_required(VERSION 2.8)

project(GPlay3d-deps)

IF(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND NOT ANDROID)
    SET(LINUX 1)
ENDIF(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND NOT ANDROID)

if(NOT ANDROID AND NOT IOS)
    # Skip any introspection for cross-compiling targets
    include(${PROJECT_SOURCE_DIR}/cmake/TargetArch.cmake)

    set(ARCH "unknown")
    target_architecture(ARCH)
endif(NOT ANDROID AND NOT IOS)

set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/out/external-deps")

if(NOT ANDROID AND NOT IOS)
    # Android and iOS toolchains will set their own output path
    IF(APPLE)
        set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/out/external-deps/lib/macosx/${ARCH}")
    ELSE()
        string(TOLOWER ${CMAKE_SYSTEM_NAME} LOWER_SYSTEM_NAME)
        set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/out/external-deps/lib/${LOWER_SYSTEM_NAME}/${ARCH}")
    ENDIF(APPLE)
endif(NOT ANDROID AND NOT IOS)

IF(NOT WIN32)
    # For Windows, let the user pick since we need to generate both Debug and
    # Release variants of all the libs.
    SET(CMAKE_BUILD_TYPE "Release")
ENDIF(NOT WIN32)
#
#file(MAKE_DIRECTORY ${LIBRARY_OUTPUT_PATH})
#message(STATUS "Directory: ${LIBRARY_OUTPUT_PATH}")

add_subdirectory(third-party/bgfx)


set(BUILD_TARGETS
        bgfx
        bimg
        bx
        brtshaderc
        )


if(WIN32)
    set(GLAY3D_DEPS gplay3d-deps.lib)
else(WIN32)
    set(GLAY3D_DEPS libgplay3d-deps.a)
endif(WIN32)

if(LINUX OR ANDROID)
    add_custom_command(
            OUTPUT ${GLAY3D_DEPS}
            COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/cmake/gnu-amalgamate.sh ${CMAKE_AR} ${LIBRARY_OUTPUT_PATH}
    )
elseif(APPLE)
    add_custom_command(
            OUTPUT ${GLAY3D_DEPS}
            COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/cmake/xcode-amalgamate.sh ${LIBRARY_OUTPUT_PATH}
    )
else()
    # Windows
    add_custom_command(
            OUTPUT ${GLAY3D_DEPS}
            COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/cmake/msvc-amalgamate.bat ${LIBRARY_OUTPUT_PATH}/${CMAKE_CFG_INTDIR}
    )
endif(LINUX OR ANDROID)

add_custom_target( gplay3d-deps ALL DEPENDS ${GLAY3D_DEPS})
add_dependencies( gplay3d-deps ${BUILD_TARGETS})

SET_DIRECTORY_PROPERTIES(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${LIBRARY_OUTPUT_PATH}/${GLAY3D_DEPS})
