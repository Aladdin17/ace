cmake_minimum_required( VERSION 3.8 )
project(
	aLibrary
	VERSION 0.0.1
	LANGUAGES C
)

set( CMAKE_POLICY_DEFAULT_CMP0076 NEW )

add_library( ${PROJECT_NAME} STATIC )
set_target_properties(
	${PROJECT_NAME}
	PROPERTIES
		C_STANDARD 11
		C_STANDARD_REQUIRED ON
)

target_compile_definitions(
	${PROJECT_NAME}
	PUBLIC
		$<$<CONFIG:Debug>:AL_DEBUG>
		$<$<CONFIG:RelWithDebInfo>:AL_DEBUG>
		$<$<BOOL:${WIN32}>:AL_PLATFORM_WINDOWS>
)

target_compile_options(
	${PROJECT_NAME}
	PRIVATE
		# Set gcc compiler warnings
		$<$<C_COMPILER_ID:GNU>: -Wall -Wextra -Wpedantic>
		$<$<C_COMPILER_ID:GNU>: -std=c11> # set c11 standard
		$<$<C_COMPILER_ID:GNU>: -Wno-unknown-pragmas> # disable warnings from external headers

		# Set MSVC compiler warnings
		$<$<C_COMPILER_ID:MSVC>: /W4>
		$<$<C_COMPILER_ID:MSVC>: /std:c11> # set c11 standard
		$<$<C_COMPILER_ID:MSVC>: /external:anglebrackets /external:W0> # disable warnings from external headers
)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC # include directories for public headers
		${CMAKE_CURRENT_SOURCE_DIR}/include
	PRIVATE # include directories for private headers
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_subdirectory( src )

option( AL_BUILD_TESTS "Build aLibrary test suites" OFF )
# if( AL_BUILD_TESTS )
	# create a net project for the test suite
	# define the macro AL_NO_STATIC_FUNCTIONS to expose static functions for testing
	# add_subdirectory( test )
# endif()
