cmake_minimum_required(VERSION 3.13)

project(app_c C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

set(TOUCH_FILE "../lib/utils/timestamp.c")
add_custom_target(timestamp 
    COMMAND touch ${TOUCH_FILE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )


include_directories(../lib)
include_directories(../app-c)
add_executable(${PROJECT_NAME}
    main.c
    ../lib/app/canvas.c
    ../lib/app/screen.c
    ../lib/app/interface.c
    ../lib/app/grain.c

    ../lib/xsp/display.c
    ../lib/xsp/socket_display.c
    ../lib/xsp/ctrl_panel.c
    ../lib/xsp/nv_memory.c

    ../lib/xal/socket_server.c

    ../lib/utils/bg_log.c
    ../lib/utils/timestamp.c
)

add_dependencies(${PROJECT_NAME} timestamp)