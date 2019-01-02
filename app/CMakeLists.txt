

cmake_minimum_required(VERSION 3.4.1)

include_directories(src/main/cpp/include)


add_library(
             native-lib


             SHARED


             src/main/cpp/native-lib.cpp )



set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/src/main/cpp/libs/${ANDROID_ABI}")



target_link_libraries(
                       native-lib

                       avcodec avfilter avformat avutil swresample swscale

                       log )