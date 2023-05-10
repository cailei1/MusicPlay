# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.18.1)

project("musicplay")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
#判断编译器类型,如果是gcc编译器,则在编译选项中加入c++11支持
if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
    message(STATUS "optional:-std=c++11")
endif (CMAKE_COMPILER_IS_GNUCXX)

# 引入头文件
#include_directories(${CMAKE_SOURCE_DIR}/../jniLibs/include)
#include_directories(${CMAKE_SOURCE_DIR}/../jniLibs/other)
include_directories(include)
include_directories(other)
#FFMpeg配置
#FFmpeg配置目录
#set(distribution_DIR ${CMAKE_SOURCE_DIR}/../jniLibs)

set(distribution_DIR ${CMAKE_SOURCE_DIR}/../)

#AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/src/main/cpp SRC_LIST)
file(GLOB_RECURSE SRC_DIR_LIST "../cpp/*.cpp" "../cpp/*.c")
# Declares and names the project.

message(STATUS "SRC_DIR_LIST = ${SRC_DIR_LIST}")
# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.


# 编解码(最重要的库)
add_library(
        avcodec
        SHARED
        IMPORTED)
set_target_properties(
        avcodec
        PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/../../libs/${ANDROID_ABI}/libavcodec.so)

 # 设备信息
add_library(
        avdevice
        SHARED
        IMPORTED)
set_target_properties(
        avdevice
        PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/../../libs/${ANDROID_ABI}/libavdevice.so)


# 滤镜特效处理库
add_library(
        avfilter
        SHARED
        IMPORTED)
set_target_properties(
        avfilter
        PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/../../libs/${ANDROID_ABI}/libavfilter.so)

# 封装格式处理库
add_library(
        avformat
        SHARED
        IMPORTED)
set_target_properties(
        avformat
        PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/../../libs/${ANDROID_ABI}/libavformat.so)

# 工具库(大部分库都需要这个库的支持)
add_library(
        avutil
        SHARED
        IMPORTED)
set_target_properties(
        avutil
        PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/../../libs/${ANDROID_ABI}/libavutil.so)


# 后期处理
#add_library(
#        postproc
#        SHARED
#        IMPORTED)
#set_target_properties(
#        postproc
#        PROPERTIES IMPORTED_LOCATION
#        ${distribution_DIR}/../../libs/${ANDROID_ABI}/libpostproc.so)


# 音频采样数据格式转换库
add_library(
        swresample
        SHARED
        IMPORTED)
set_target_properties(
        swresample
        PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/../../libs/${ANDROID_ABI}/libswresample.so)

# 视频像素数据格式转换
add_library(
        swscale
        SHARED
        IMPORTED)
set_target_properties(
        swscale
        PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/../../libs/${ANDROID_ABI}/libswscale.so)

add_library( # Sets the name of the library.
        music-player

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        native-lib.cpp
        test.c
#        AudioTrackPlayer.cpp
        CLFFmpeg.cpp
        call_java_back.cpp
        ${SRC_DIR_LIST}
#        ffmpeg_opt.c
#        ffmpeg_mod.c
#        ffmpeg_filter.c
        )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        music-player
        avcodec avfilter avformat avutil swresample swscale avdevice
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})