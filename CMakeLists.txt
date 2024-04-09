cmake_minimum_required(VERSION 3.16)
project(VolumeReg VERSION 0.0.1)

set(ITK_DIR "D:\\BP\\itk\\ITK-build")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(VolumeRegistration "src/PNGWriter.cpp" "include/VolumeRegistration.h" "include/DcmReader.h" "src/DcmReader.cpp" "include/PngWriter.h")

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})
target_link_libraries(VolumeRegistration PUBLIC ${ITK_LIBRARIES})

target_include_directories(VolumeRegistration PUBLIC "include")