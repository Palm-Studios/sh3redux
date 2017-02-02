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
set(ASSERTION_BEHAVIOR_DEFAULT "Log and ask")
if("${CMAKE_BUILD_TYPE}" STREQUAL "Release" OR "${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
	set(ASSERTION_BEHAVIOR_DEFAULT "Disable")
endif()
set(ASSERTION_BEHAVIOR "${ASSERTION_BEHAVIOR_DEFAULT}" CACHE STRING "What to do when an assertion fails.")
set_property(CACHE ASSERTION_BEHAVIOR PROPERTY STRINGS "Disable" "Log and abort" "Log and continue" "Log and ask")

if("${ASSERTION_BEHAVIOR}" STREQUAL "Disable")
	add_definitions("-DASSERT_OFF")
elseif("${ASSERTION_BEHAVIOR}" STREQUAL "Log and abort")
	#default, no special defines required
elseif("${ASSERTION_BEHAVIOR}" STREQUAL "Log and continue")
	add_definitions("-DASSERT_CONTINUE")
elseif("${ASSERTION_BEHAVIOR}" STREQUAL "Log and ask")
	add_definitions("-DASSERT_ASK_MSGBOX" "-DASSERT_ASK_STDERR")
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
	list(APPEND CXX_FLAGS
		"-pedantic"
		"-Wall"
		"-Wextra"
		"-Wsign-compare"
		"-Wold-style-cast"
		"-Wdeprecated"
		"-Wsign-conversion"
		"-Wconversion"
		"-Wnon-virtual-dtor"
		"-Wundef"
		"-Wfloat-equal"
		"-Wunreachable-code"
		"-Wswitch-default"
		"-Weffc++"
	)
	string(REPLACE ";" " " CXX_FLAGS "${CXX_FLAGS}")
	set(CXX_FLAGS "${CXX_FLAGS}" CACHE INTERNAL "Additional compiler flags to use.")
	
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

set(CMAKE_CXX_FLAGS "${CXX_FLAGS} ${CMAKE_CXX_FLAGS}")

add_subdirectory(source)

find_package(Doxygen)

if(DOXYGEN_FOUND)
	add_custom_target(doc
		"${DOXYGEN_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile"
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		COMMENT "Generating documentation..."
		VERBATIM
	)
else()
	message(WARNING "Doxygen not found. Target 'doc' not available.")
endif()
