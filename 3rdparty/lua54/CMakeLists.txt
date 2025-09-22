cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(lua LANGUAGES C)

add_library(lua STATIC
	src/lapi.c
	src/lcode.c
	src/lctype.c
	src/ldebug.c
	src/ldo.c
	src/ldump.c
	src/lfunc.c
	src/lgc.c
	src/llex.c
	src/lmem.c
	src/lobject.c
	src/lopcodes.c
	src/lparser.c
	src/lstate.c
	src/lstring.c
	src/ltable.c
	src/ltm.c
	src/lundump.c
	src/lvm.c
	src/lzio.c
	src/lauxlib.c
	src/lbaselib.c
	src/ldblib.c
	src/lmathlib.c
	src/ltablib.c
	src/lstrlib.c
	src/lutf8lib.c
	src/lcorolib.c
	src/linit.c
)

target_include_directories(lua PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(lua PRIVATE m)