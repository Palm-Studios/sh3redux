cmake_minimum_required(VERSION 3.1)

execute_process(COMMAND cmake -E echo *
	OUTPUT_FILE ".gitignore")

project(sh3rd CXX)

if(WIN32)
	enable_language(RC)
endif()

if("${CMAKE_SIZEOF_VOID_P}" EQUAL 8)
	set(BINARY_RESOURCE_FILE "resource64.rc")
	add_definitions(-DSH3_64)
elseif("${CMAKE_SIZEOF_VOID_P}" EQUAL 4)
	set(BINARY_RESOURCE_FILE "resource32.rc")
else()
	message(SEND_ERROR "Could not detect system bitness.")
endif()

include(CheckCXXCompilerFlag)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Type of build." FORCE)
	if(NOT CMAKE_CONFIGURATION_TYPES)
		set(CMAKE_CONFIGURATION_TYPES "Debug" "Release" "MinSizeRel" "RelWithDebugInfo")
	endif()
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${CMAKE_CONFIGURATION_TYPES})
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
	set(CXX_FLAGS "-Wall -Wextra -pedantic -masm=intel" CACHE INTERNAL "Additional compiler flags to use.")
	
	option(CXX_FORCE_COLOR "Force compiler to use colored output (if available)." ON)
	if(CXX_FORCE_COLOR)
		set(COLOR_DIAG_FLAGS "fcolor-diagnostics" "fdiagnostics-color=always")
		foreach(COLOR_DIAG_FLAG ${COLOR_DIAG_FLAGS})
			set(TEST_NAME "HAVE_${COLOR_DIAG_FLAG}_FLAG")
			check_cxx_compiler_flag("-${COLOR_DIAG_FLAG}" ${TEST_NAME})
			if(${TEST_NAME})
				message(STATUS "Forcing colored compiler output")
				set(CXX_FLAGS "${CXX_FLAGS} -${COLOR_DIAG_FLAG}")
			endif()
		endforeach()
	endif()
	
	set(BUILD_EXTRA_WARNINGS "Some" CACHE STRING "How many extra warnings to enable.")
	set_property(CACHE BUILD_EXTRA_WARNINGS PROPERTY STRINGS "None" "Some" "Many")
	
	# Some warnings that are of absolutely no interest
	set(WARN_NOT_FLAGS "c++98-compat" "c++98-compat-pedantic" "exit-time-destructors" "ignored-qualifiers" CACHE STRING "Warning-flags to disable.")
	if(NOT ${BUILD_EXTRA_WARNINGS} STREQUAL "Many")
		# These warnings often produce much uninteresting output
		list(APPEND WARN_NOT_FLAGS "padded")
	endif()
	
	if(NOT ${BUILD_EXTRA_WARNINGS} STREQUAL "None")
		set(WARN_EVERYTHING_FLAG "Weverything" CACHE STRING "The flag that enables all warnings.")
		check_cxx_compiler_flag("-${WARN_EVERYTHING_FLAG}" HAVE_${WARN_EVERYTHING_FLAG}_FLAG)
		if(HAVE_${WARN_EVERYTHING_FLAG}_FLAG)
			set(CXX_FLAGS "${CXX_FLAGS} -${WARN_EVERYTHING_FLAG}")
		endif()
	endif()
	
	foreach(WARN_FLAG ${WARN_NOT_FLAGS})
		string(REPLACE "+" "x" TEST_NAME "HAVE_W${WARN_FLAG}_FLAG")
		check_cxx_compiler_flag("-W${WARN_FLAG}" ${TEST_NAME})
		if(${TEST_NAME})
			message(STATUS "Disabling -W${WARN_FLAG}")
			set(CXX_FLAGS "${CXX_FLAGS} -Wno-${WARN_FLAG}")
		endif()
	endforeach()
elseif(MSVC)
	set(CXX_FLAGS "/W4")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin")

string(CONCAT CMAKE_CXX_FLAGS "${CXX_FLAGS}" "${CMAKE_CXX_FLAGS}")

add_subdirectory(source)
