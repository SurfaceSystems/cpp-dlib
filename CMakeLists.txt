cmake_minimum_required(VERSION 3.10)
project(dlib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SRCS "src/*.cpp")

add_executable(dlib ${SRCS})

target_include_directories(dlib PRIVATE include/)
target_include_directories(dlib PRIVATE external/)

find_package(CURL REQUIRED)

target_link_libraries(dlib PRIVATE CURL::libcurl)

if(MSVC)
    add_compile_options(/W4 /WX)  # Equivalent to -Wall -Wextra
else()
    add_compile_options(-Wall -Wextra)
endif()
