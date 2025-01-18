cmake_minimum_required(VERSION 3.26)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w")

project (client VERSION "0.1.1" LANGUAGES C)
add_executable(${PROJECT_NAME} main.c client.c socketutil.c helper.c)
