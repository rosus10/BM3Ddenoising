cmake_minimum_required(VERSION 3.5)
project(BCDenoising)

find_package(OpenCV REQUIRED)

add_executable(BCDenoising
    BCDenoising/BCDenoising/BCDenoising.cpp
    BCDenoising/BCDenoising/BM3DDenoiser.cpp
    BCDenoising/BCDenoising/Blockmatching.cpp
)

target_include_directories(BCDenoising PRIVATE
    BCDenoising/BCDenoising
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(BCDenoising PRIVATE ${OpenCV_LIBS})
