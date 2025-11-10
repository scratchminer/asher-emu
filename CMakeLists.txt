# Asher, a Playdate emulator
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

# if(APPLE)
#	set(CMAKE_OSX_ARCHITECTURES "arm64")
# endif()

# support for stdbool.h
set(CMAKE_C_STANDARD 99)

set(ASHER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(ASHER_BINARY_DIR ${CMAKE_BINARY_DIR})
set(ASHER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

option(BUILD_WITH_UNICORN "Build with Unicorn (required for native games)" ON)
# option(BUILD_WITH_LUA "Build with Lua fork (required for Lua games)" ON)

project(Asher VERSION "0.0.1" LANGUAGES C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(FFI REQUIRED QUIET IMPORTED_TARGET libffi)

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ASHER_BINARY_DIR})

# For the MSVC (Visual C) compiler
if(MSVC)
	# Microsoft deprecation warnings
	add_definitions("/D\"_CRT_SECURE_NO_WARNINGS\"")
	add_definitions("/D\"_CRT_NONSTDC_NO_DEPRECATE\"")
	
	# For more than one build configuration, all of them should be in the binary folder
	foreach(OUTPUT_CONFIG ${CMAKE_CONFIGURATION_TYPES})
		string(TOUPPER ${OUTPUT_CONFIG} OUTPUT_CONFIG)
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUT_CONFIG} ${ASHER_BINARY_DIR})
	endforeach()
endif()

# DEPENDENCIES

if(BUILD_WITH_UNICORN)
	# Unicorn engine for native code emulation
	set(UNICORN_ARCH "arm" CACHE STRING "")
	add_subdirectory(3rdparty/unicorn)
	target_compile_options(arm-softmmu PRIVATE -Wno-initializer-overrides -Wno-constant-conversion)
endif()
if(BUILD_WITH_LUA)
	# Lua fork for Lua code emulation
	add_subdirectory(3rdparty/lua54)
endif()

add_subdirectory(3rdparty/md5)

add_executable(asher
	src/main.c
	src/device.c
	src/peripherals.c
	src/peripherals/dummy.c
	src/peripherals/dvt1/dma.c
	src/peripherals/dvt1/gpio.c
	src/peripherals/dvt1/pwr.c
	src/peripherals/dvt1/rcc.c
	src/peripherals/dvt1/syscfg.c
	src/peripherals/dvt1/sysctl.c
	src/peripherals/dvt1/tim.c
)

target_include_directories(asher PRIVATE FFI_INCLUDE_DIRS INTERFACE ${ASHER_INCLUDE_DIR})
target_link_libraries(asher PRIVATE SDL3::SDL3 PkgConfig::FFI md5)

if(BUILD_WITH_UNICORN)
	target_link_libraries(asher PRIVATE unicorn)
endif()