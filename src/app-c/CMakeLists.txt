cmake_minimum_required(VERSION 3.13)

project(app_c C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)



include_directories(../lib)
add_executable(${PROJECT_NAME}
    main.c
)
