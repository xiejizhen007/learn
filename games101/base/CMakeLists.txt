cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME base)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES src/*.cpp src/*.c)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} GLEW glfw GL X11 pthread Xrandr Xi ${CMAKE_DL_LIBS})