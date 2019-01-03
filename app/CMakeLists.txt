

cmake_minimum_required(VERSION 3.4.1)

include_directories(src/main/cpp/include)

file(GLOB source_file src/main/cpp/*.cpp)
add_library(
             native-lib


             SHARED


             src/main/cpp/native-lib.cpp

             ${source_file}
              )



set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/src/main/cpp/libs/${ANDROID_ABI}")



target_link_libraries(
                       native-lib

                       avcodec avfilter avformat avutil swresample swscale

                       log )