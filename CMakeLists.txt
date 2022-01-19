cmake_minimum_required(VERSION 3.20)
project(CGchuyan)

set(CMAKE_CXX_STANDARD 14)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

add_executable(CGchuyan main.cpp stb_image.cpp ${PROJECT_SOURCE_DIR}/src/glad.c include/camera.h include/texture.h)

target_link_libraries(CGchuyan ${PROJECT_SOURCE_DIR}/lib/glfw3.dll)