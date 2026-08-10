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
    target_compile_definitions(dlib PRIVATE 
        _CRT_SECURE_NO_WARNINGS
        _SCL_SECURE_NO_WARNINGS
    )
    
    target_compile_options(dlib PRIVATE 
        /W4 
        /wd4267
        /wd4996
    )
else()
    add_compile_options(-Wall -Wextra)
endif()
