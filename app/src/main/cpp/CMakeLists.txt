

cmake_minimum_required(VERSION 3.4.1)
# set targetPlatform, will be passed in from gradle when this sample is completed
# openGL Supportability
# platform         status
#   (0 12)           ES2/ES3 not supported
#   [12, 18)         ES2 only; for ES3, app do dynamic load/detection
#                    this applies to the situations that:
#                        - minimum API is set to less than 18. In this case
#                          there is no ES3 header/lib support inside NDK
#                        - the built APK might be running on newer API phones
#                    with dynamic loading of ES3, the same APK would still be able
#                    to use ES3. Otherwise, app would stuck with ES2 even phone is
#                    is newer than the minimum API level (for example, Android-27 etc).
#
#   [18, 24)         ES2 & ES3
#                    If app is built to only support API-18 or later,
#                    set minimum api level to 18 is good enough, NDK supprts ES3
#                    with the right header and lib files. No need to use ES3 dynamic
#                    detection.
#   [24, infinite)   ES2 & ES3 & Vulkan
option(HELLO_BGFX_BUILD_SHADERS "" OFF)
set(BGFX_AMALGAMATED      ON CACHE INTERNAL "" FORCE)
set(BGFX_BUILD_EXAMPLES   OFF CACHE INTERNAL "" FORCE)
set(BGFX_CONFIG_DEBUG     OFF CACHE INTERNAL "" FORCE)
set(BGFX_CUSTOM_TARGETS   OFF CACHE INTERNAL "" FORCE)
set(BGFX_INSTALL          ON CACHE INTERNAL "" FORCE)
set(BGFX_INSTALL_EXAMPLES OFF CACHE INTERNAL "" FORCE)
set(BGFX_USE_DEBUG_SUFFIX OFF CACHE INTERNAL "" FORCE)
set(BGFX_USE_OVR          OFF CACHE INTERNAL "" FORCE)
set(BUILD_SHARED_LIBS     OFF CACHE INTERNAL "" FORCE)
set(BX_AMALGAMATED        OFF CACHE INTERNAL "" FORCE)
if (HELLO_BGFX_BUILD_SHADERS)
    set(BGFX_BUILD_TOOLS ON CACHE INTERNAL "" FORCE)
else()
    set(BGFX_BUILD_TOOLS OFF CACHE INTERNAL "" FORCE)
endif()
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/bgfx.cmake)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti -fno-exceptions -Wall")
if (${ANDROID_PLATFORM_LEVEL} LESS 12)
message(FATAL_ERROR "OpenGL 2 is not supported before API level 11 \
                      (currently using ${ANDROID_PLATFORM_LEVEL}).")
return()
elseif (${ANDROID_PLATFORM_LEVEL} LESS 18)
add_definitions("-DDYNAMIC_ES3")
set(GL3STUB_SRC gl3stub.c)
set(OPENGL_LIB GLESv2)
else ()
set(OPENGL_LIB GLESv3)
endif (${ANDROID_PLATFORM_LEVEL} LESS 12)


add_definitions(-DHELLO_BGFX_RUNTIME_RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/runtime")

if (HELLO_BGFX_BUILD_SHADERS)
    add_custom_target(
            build-shaders
            COMMAND $<TARGET_FILE:shaderc> -f ${CMAKE_CURRENT_SOURCE_DIR}/shaders/vs_blinnphong.sc -o ${CMAKE_CURRENT_SOURCE_DIR}/runtime/glsl/vs_blinnphong.bin --type v -i ${BGFX_DIR}/src --varyingdef ${CMAKE_CURRENT_SOURCE_DIR}/shaders/varying_blinnphong.def.sc
            COMMAND $<TARGET_FILE:shaderc> -f ${CMAKE_CURRENT_SOURCE_DIR}/shaders/fs_blinnphong.sc -o ${CMAKE_CURRENT_SOURCE_DIR}/runtime/glsl/fs_blinnphong.bin --type f -i ${BGFX_DIR}/src --varyingdef ${CMAKE_CURRENT_SOURCE_DIR}/shaders/varying_blinnphong.def.sc
    )
endif()

#file(GLOB files src/*.cpp src/*.hpp src/primitives/*.cpp src/primitives/*.hpp shaders/*.sc)
#add_library(gles3jni STATIC ${files})
##target_link_libraries(hello-bgfx bgfx glfw glm libglew_static)


add_library(gles3jni STATIC
${GL3STUB_SRC}
gles3jni.cpp
RendererES2.cpp
RendererES3.cpp)

# Include libraries needed for gles3jni lib
target_link_libraries(gles3jni
${OPENGL_LIB}
 bgfx
android
EGL
log)

