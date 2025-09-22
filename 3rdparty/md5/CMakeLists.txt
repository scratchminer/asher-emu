cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(md5 LANGUAGES C)

add_library(md5 STATIC src/md5.c)

target_include_directories(md5 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)