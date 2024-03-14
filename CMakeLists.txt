cmake_minimum_required( VERSION 3.8 )
project(
	aLibrary
	VERSION 0.0.1
	LANGUAGES C CXX
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
if( AL_BUILD_TESTS )
	# modify base library config for testing
	target_compile_definitions(
		${PROJECT_NAME}
		PUBLIC
			AL_NO_STATIC_FUNCTIONS # expose static functions for testing
	)
	target_include_directories(
		${PROJECT_NAME}
		PUBLIC
			${CMAKE_CURRENT_SOURCE_DIR}/src # include src directory for testing
	)

	# import the Catch2 library
	include( FetchContent )
	FetchContent_Declare(
		Catch2
		GIT_REPOSITORY https://github.com/catchorg/Catch2.git
		GIT_TAG v3.4.0
	)

	FetchContent_MakeAvailable( Catch2 )

	# create the test project
	add_executable( ${PROJECT_NAME}_test )
	set_target_properties(
		${PROJECT_NAME}_test
		PROPERTIES
			CXX_STANDARD 11
			CXX_STANDARD_REQUIRED ON
	)

	# link Catch2, base library, and tests directory
	target_link_libraries( ${PROJECT_NAME}_test PRIVATE Catch2::Catch2WithMain )
	target_link_libraries( ${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME} )
	add_subdirectory( test )

	# automatic test discovery and registration
	list( APPEND CMAKE_MODULE_TATH ${catch2_SOURCE_DIR}/extras )
	include( CTest )
	include( Catch )
	catch_discover_tests( ${PROJECT_NAME}_test )
endif()
