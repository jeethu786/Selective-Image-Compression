set(pathToProject C:/Users/jeethender/Music/ImageCompressor)
set(pathToOpenCv C:/Users/jeethender/Music/OpenCV-android-sdk)


cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")


include_directories(${pathToOpenCv}/sdk/native/jni/include)

add_library( lib_opencv SHARED IMPORTED )

#set_target_properties(lib_opencv PROPERTIES IMPORTED_LOCATION ${pathToProject}/app/src/main/jniLibs/${ANDROID_ABI}/libopencv_java4.so)
set_target_properties(lib_opencv PROPERTIES IMPORTED_LOCATION ${pathToProject}/app/src/main/jniLibs/${ANDROID_ABI}/libopencv_java3.so)

add_library( # Sets the name of the library.
             combine
             SHARED
             combine.cpp

             )

find_library( # Sets the name of the path variable.
              combine
              log )
target_link_libraries( # Specifies the target library.
                       combine
                       lib_opencv
                       ${log-lib} )



add_library( # Sets the name of the library.
        compression
        SHARED
        compress.cpp
        )

find_library( # Sets the name of the path variable.
        compression
        log )

target_link_libraries( # Specifies the target library.
        compression
        lib_opencv
        ${log-lib} )


add_library( # Sets the name of the library.
        Huffman
        SHARED
        Huffman.cpp
        )

find_library( # Sets the name of the path variable.
        Huffman
        log )

target_link_libraries( # Specifies the target library.
        Huffman
        lib_opencv
        ${log-lib} )

add_library(
        Bitmap
        SHARED
        Bitmap.cpp
)
find_library(
        Bitmap
        log )

target_link_libraries(
        Bitmap
        lib_opencv
        ${log-lib} )

add_library(
        bitfile
        SHARED
        bitfile.cpp
)
find_library(
        bitfile
        log )

target_link_libraries(
        bitfile
        lib_opencv
        ${log-lib} )

add_library(
        DWT
        SHARED
        DWT.cpp
)
find_library(
        DWT
        log )

target_link_libraries(
        DWT
        lib_opencv
        ${log-lib} )
add_library(
        Huffman1
        SHARED
        Huffman1.cpp
)
find_library(
        Huffman1
        log )

target_link_libraries(
        Huffman1
        lib_opencv
        ${log-lib} )

