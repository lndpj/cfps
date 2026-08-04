cmake_minimum_required( VERSION 3.10 )

project(cfps VERSION 1.0
	LANGUAGES C
	HOMEPAGE_URL "https://github.com/lndpj/cfps"
	DESCRIPTION "Alarm based FPS counter and date in C"
)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

if(NOT CMAKE_GENERATOR MATCHES "^Visual Studio")
include(GNUInstallDirs)
endif()

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED OFF)
set(CMAKE_C_EXTENSIONS ON)

if( NOT CMAKE_BUILD_TYPE )
        set( CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE )
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "${CMAKE_HOST_SYSTEM_NAME} ${CMAKE_HOST_SYSTEM_PROCESSOR} ${CMAKE_C_COMPILER_ID}/${CMAKE_C_COMPILER_FRONTEND_VARIANT} => ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_PROCESSOR} ${CMAKE_C_COMPILER_ID}/${CMAKE_C_COMPILER_FRONTEND_VARIANT}")

if(CMAKE_C_COMPILER_FRONTEND_VARIANT MATCHES "GNU|AppleClang")
if( NOT CMAKE_OPTIMIZATION_LEVEL )
	set( CMAKE_OPTIMIZATION_LEVEL 3 CACHE STRING "Choose the optimization level of build." FORCE )
endif()

if(NOT CMAKE_CROSSCOMPILING)
	add_compile_options(-march=native)
endif()

if(CMAKE_SYSTEM_PROCESSOR MATCHES "x64|x86_64|AMD64")
	add_compile_options(-mfpmath=sse)
endif()

if(CMAKE_C_COMPILER_ID MATCHES "Clang")
	add_compile_options(-Wno-gnu-alignof-expression)
endif()

	add_compile_options(-fopenmp)
if(NOT CMAKE_C_COMPILER_ID MATCHES "AppleClang")
	add_compile_options(-fopenmp-simd)
endif()

add_compile_options(-O${CMAKE_OPTIMIZATION_LEVEL})
add_compile_options(-ftree-vectorize)
add_compile_options(-D_FILE_OFFSET_BITS=64)
add_compile_options(-fdata-sections)
add_compile_options(-fpermissive)
add_compile_options(-ffunction-sections)
add_compile_options(-Wno-array-bounds)
add_compile_options(-Wno-unused-parameter)
add_compile_options(-Wno-unused-result)
add_compile_options(-Wno-pedantic)
add_compile_options(-Wno-narrowing)
add_link_options(-Wl,--gc-sections)

if( CMAKE_BUILD_TYPE MATCHES Debug )
	add_compile_options(-g)
endif()
if(CMAKE_BUILD_TYPE MATCHES ReleaseStrip)
	add_link_options(-Wl,-s)
endif()

elseif(CMAKE_C_COMPILER_FRONTEND_VARIANT MATCHES "MSVC")
	if( NOT CMAKE_OPTIMIZATION_LEVEL )
		set( CMAKE_OPTIMIZATION_LEVEL 2 CACHE STRING "Choose the optimization level." FORCE )
	endif()
	add_compile_options(/O${CMAKE_OPTIOMIZATION_LEVEL})
	string(REPLACE "/RTC1" "" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
	string(REPLACE "/RTC1" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
	add_compile_options(/openmp:experimental)
	add_compile_options(/Zc:__cplusplus)
	add_compile_options(/std:c++latest)
	add_compile_options(/Zc:preprocessor)
	add_compile_options(/Z7)
	add_compile_options(/Gy)					# Function-level linking (puts each function in its own section)
	add_compile_options(/Gw)					# Optimize global data (puts each variable in its own section)
	if (NOT CMAKE_CROSSCOMPILING)
		add_compile_definitions(_CRT_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_DEPRECATE _WIN32)
		if (CMAKE_VS_PLATFORM_NAME MATCHES "x64|Win32")
			#add_compile_options(/Gv)
		endif()
	endif()
	if (CMAKE_BUILD_TYPE MATCHES ReleaseStrip)
		add_link_options(/DEBUG:NONE)
		add_link_options(/OPT:REF)				# Eliminate unreferenced functions and data
		add_link_options(/VERBOSE:REF)			# Print out everything the linker throws away
	endif()
	if (CMAKE_BUILD_TYPE MATCHES Debug)
		add_compile_options(/Zi)
		add_link_options(/DEBUG:FULL)
	endif()
	add_link_options(/INCREMENTAL:NO)
else()
	message(FATAL_ERROR "C++ Compiler Frontend Variant not supported!")
endif()



if( NOT CMAKE_BUILD_TYPE )
        set( CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE )
endif()

include_directories( ${PROJECT_NAME} $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include> )
add_executable( ${PROJECT_NAME} src/cfps.c )
target_link_libraries( ${PROJECT_NAME} m )

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
