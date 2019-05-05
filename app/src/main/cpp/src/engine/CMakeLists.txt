set(THIRDPARTY_INC
    ${THIRDPARTY_DIR}/glm
    ${BGFX_INC})


set(ENGINE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(LOCAL_ENGINE_DIR ${LOCAL_GENERATED_DIR}/engine)


if(${CMAKE_HOST_WIN32})
    set(THIRDPARTY_BIN_DIR ${THIRDPARTY_DIR}/bin/win32)
elseif(${CMAKE_HOST_APPLE})
    set(THIRDPARTY_BIN_DIR ${THIRDPARTY_DIR}/bin/osx)
elseif(${CMAKE_HOST_UNIX})
    set(THIRDPARTY_BIN_DIR ${THIRDPARTY_DIR}/bin/linux)
endif()


if(ANDROID)
    find_library(LOG_LIB log)
    find_library(GLES2_LIB GLESv2)
    find_library(GLES1_LIB GLESv1_CM)
    find_library(EGL_LIB EGL)
    find_library(ANDROID_LIB android)

    set(FRAMEWORK_LIBRS
        ${ANDROID_LIB}
        ${GLES2_LIB}
        ${GLES1_LIB}
        ${EGL_LIB}
        ${LOG_LIB})
endif()

# all source code in subdirectories for ae target
if(IOS)
    file(GLOB_RECURSE src_files 
        ${ENGINE_DIR}/*.cpp
        ${ENGINE_DIR}/*.cc
        ${ENGINE_DIR}/*.c++
        ${ENGINE_DIR}/*.c
        ${ENGINE_DIR}/*.mm
        ${ENGINE_DIR}/*.m
        ${ENGINE_DIR}/*.cpb
        ${ENGINE_DIR}/*.h
        ${ENGINE_DIR}/*.hpp)
else()
    file(GLOB_RECURSE src_files 
        ${ENGINE_DIR}/*.cpp
        ${ENGINE_DIR}/*.cc
        ${ENGINE_DIR}/*.c++
        ${ENGINE_DIR}/*.c
        ${ENGINE_DIR}/*.cpb
        ${ENGINE_DIR}/*.h
        ${ENGINE_DIR}/*.hpp)
endif()


# all source code in generated/engine for ae target
file(GLOB_RECURSE gen_src_files
    ${LOCAL_ENGINE_DIR}/*.cpp
    ${LOCAL_ENGINE_DIR}/*.cc
    ${LOCAL_ENGINE_DIR}/*.c++
    ${LOCAL_ENGINE_DIR}/*.c
    ${LOCAL_ENGINE_DIR}/*.cpb
    ${LOCAL_ENGINE_DIR}/*.h
    ${LOCAL_ENGINE_DIR}/*.hpp)


# all third party source code in subdirectories for ae target
set(third_src_files
    ${bgfx_src_files})
    

if(ANDROID)
    set(ANDROID_JNI ${APP_DIR}/android/duar/src/main/jni)
    file(GLOB_RECURSE android_src_files
        ${ANDROID_JNI}/*.cpp
        ${ANDROID_JNI}/*.cc
        ${ANDROID_JNI}/*.c++
        ${ANDROID_JNI}/*.c
        ${ANDROID_JNI}/*.cpb
        ${ANDROID_JNI}/*.h
        ${ANDROID_JNI}/*.hpp)

    add_library(${TARGET_NAME}
        SHARED
        ${src_files}
        ${third_src_files}
        ${gen_src_files}
        ${android_src_files})
else()
    add_library(${TARGET_NAME}
        STATIC
        ${src_files}
        ${third_src_files}
        ${gen_src_files})
endif()


if(NOT ANDROID)
    # source group for all ae source files
    source_group(
        TREE ${ENGINE_DIR}
        PREFIX "src"
        FILES ${src_files})

    # source group for third party source files
    source_group(
        TREE ${THIRDPARTY_DIR}
        PREFIX "thirdparty"
        FILES ${third_src_files})

endif()

# where shall we find our headers

if(ANDROID)
    set(ARCORE ${THIRDPARTY_DIR}/arcore-native/include)
    target_include_directories(
        ${TARGET_NAME} PRIVATE 
        ${ENGINE_DIR}
        ${LOCAL_ENGINE_DIR}
        ${THIRDPARTY_INC}
        ${ANDROID_TOOLCHAIN_INC}
        ${ARCORE})
else() 
    target_include_directories(
        ${TARGET_NAME} PRIVATE 
        ${ENGINE_DIR}
        ${LOCAL_ENGINE_DIR}
        ${THIRDPARTY_INC})
endif()

# if(IOS)
#     set(THIRDPARTY_STATIC_LIBS
#         ${THIRDPARTY_LIB_DIR}/libtoluapp.a
#         ${THIRDPARTY_LIB_DIR}/libcJSON.a
#         ${THIRDPARTY_LIB_DIR}/libcapnp-json.a
#         ${THIRDPARTY_LIB_DIR}/libcapnp.a
#         ${THIRDPARTY_LIB_DIR}/libkj.a)
# else

if(ANDROID)
 #   set(THIRDPARTY_STATIC_LIBS
#        ${THIRDPARTY_LIB_DIR}/${ANDROID_ABI}/liblua.a
#        ${THIRDPARTY_LIB_DIR}/${ANDROID_ABI}/libcJSON.a
#        ${THIRDPARTY_LIB_DIR}/${ANDROID_ABI}/libcapnp-json.a
#        ${THIRDPARTY_LIB_DIR}/${ANDROID_ABI}/libcapnp.a
#        ${THIRDPARTY_LIB_DIR}/${ANDROID_ABI}/libkj.a
#    ${THIRDPARTY_LIB_DIR}/${ANDROID_ABI}/libarcore_sdk_c.so)
endif()

target_link_libraries(${TARGET_NAME} ${THIRDPARTY_STATIC_LIBS})
target_link_libraries(${TARGET_NAME} ${FRAMEWORK_LIBRS})



# build shader bundle

file(GLOB_RECURSE shader_files
        ${ENGINE_DIR}/shader/shaderdb.config
        ${ENGINE_DIR}/shader/*.shd)

if(NOT ANDROID)
        source_group(
            TREE ${ENGINE_DIR}
            FILES ${shader_files})
endif()

file(MAKE_DIRECTORY ${ASSETS_DIR}/shader)

if(ANDROID)
    file(MAKE_DIRECTORY ${LOCAL_DIR}/build/shader/es2)
    set(SHADER_PLATFORM android)
    set(SHADER_PROFILES es2)
elseif(IOS)
    file(MAKE_DIRECTORY ${LOCAL_DIR}/build/shader/es3)
    file(MAKE_DIRECTORY ${LOCAL_DIR}/build/shader/metal)
    set(SHADER_PLATFORM ios)
    set(SHADER_PROFILES metal)
endif()

#add_custom_target(ShaderDB
#    COMMAND "python"
#    ${SCRIPT_DIR}/shader/db_build.py
#    -w ${PROJECT_DIR}
#    --platform ${SHADER_PLATFORM}
#    --profiles ${SHADER_PROFILES}
#    COMMENT "build shader bundle"
#    DEPENDS ${ENGINE_DIR}/shader
#    VERBATIM
#    SOURCES ${shader_files})
#
#add_dependencies(AREngine ShaderDB)

if(IOS)
    set_target_properties(
        ${TARGET_NAME} PROPERTIES
        XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET 8.0
        XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC NO
        XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++14"
        XCODE_ATTRIBUTE_ARCHS "armv7 arm64"
        # XCODE_ATTRIBUTE_GCC_ENABLE_CPP_RTTI NO
        # XCODE_ATTRIBUTE_GCC_ENABLE_CPP_EXCEPTIONS NO
        XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS[variant=Release] NO
        XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS[variant=Debug] YES
        XCODE_ATTRIBUTE_GCC_OPTIMIZATION_LEVEL[variant=Release] s
        XCODE_ATTRIBUTE_OTHER_CFLAGS[variant=Release] "-fembed-bitcode"
        XCODE_ATTRIBUTE_OTHER_CPLUSPLUSFLAGS[variant=Debug] "-fvisibility=hidden -fvisibility-inlines-hidden -DDUAR_IOS -DDUAR_DEBUG -D_DEBUG -DBGFX_CONFIG_DEBUG -Wno-reorder"
        XCODE_ATTRIBUTE_OTHER_CPLUSPLUSFLAGS[variant=Release] "-fvisibility=hidden -fvisibility-inlines-hidden -DDUAR_IOS -DDUAR_RELEASE -Wno-reorder"
        XCODE_ATTRIBUTE_LIBRARY_SEARCH_PATHS "${THIRDPARTY_LIB_DIR}")
endif()
