cmake_minimum_required(VERSION 3.8)
project(circle_detection VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(OpenCV REQUIRED)

add_executable(cirdet "")
target_sources(cirdet
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    )

target_include_directories(cirdet
    PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(cirdet ${OpenCV_LIBS})