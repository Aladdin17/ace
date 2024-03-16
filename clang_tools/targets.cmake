# Additional target to perform clang-format/clang-tidy run
# Requires clang-format and clang-tidy

#---------------------------------------------------------------------------------------------------
# Clang-Format
#---------------------------------------------------------------------------------------------------
find_program( CLANG_FORMAT_EXE NAMES clang-format )
if( NOT CLANG_FORMAT_EXE )
	message( STATUS "clang-format not found" )
else()
	execute_process(
		COMMAND ${CLANG_FORMAT_EXE} --version
		OUTPUT_VARIABLE CLANG_FORMAT_VERSION_OUTPUT
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)

	set( CLANG_FORMAT_MIN_VERSION "18.0.0" )
	string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+" CLANG_FORMAT_VERSION ${CLANG_FORMAT_VERSION_OUTPUT})
	if( CLANG_FORMAT_VERSION VERSION_LESS ${CLANG_FORMAT_MIN_VERSION} )
		message( STATUS "clang-format version must be at least ${CLANG_FORMAT_MIN_VERSION}" )
		message( STATUS "\tclang-format version found: ${CLANG_FORMAT_VERSION}" )
	else()
		message( STATUS "clang-format version: ${CLANG_FORMAT_VERSION}" )
		file(
			GLOB_RECURSE
			FORMAT_SOURCE_FILES
			${CMAKE_SOURCE_DIR}/src/*.c
			${CMAKE_SOURCE_DIR}/src/*.h
			${CMAKE_SOURCE_DIR}/include/*.h
		)

		add_custom_target(
			clang-format
			COMMAND ${CLANG_FORMAT_EXE}
				-i
				--style=file:${CMAKE_SOURCE_DIR}/clang_tools/.clang-format
				${FORMAT_SOURCE_FILES}
		)
	endif()
endif()


#---------------------------------------------------------------------------------------------------
# Clang-Tidy
#---------------------------------------------------------------------------------------------------
find_program( CLANG_TIDY_EXE NAMES clang-tidy )
if( NOT CLANG_TIDY_EXE )
	message( STATUS "clang-tidy not found" )
else()
	message( STATUS "clang-tidy version: " )
	set( CLANG_TIDY_COMPATIBLE_GENERATORS "Ninja" "Unix Makefiles" "MinGW Makefiles" )
	list( FIND CLANG_TIDY_COMPATIBLE_GENERATORS "${CMAKE_GENERATOR}" _index )
	if( ${_index} EQUAL -1 )
		message( STATUS "Clang-tidy is not compatible with the current generator: ${CMAKE_GENERATOR}" )
	else()
		file(
			GLOB_RECURSE
			TIDY_SOURCE_FILES
			${CMAKE_SOURCE_DIR}/src/*.c
		)

		add_custom_target(
			clang-tidy
			COMMAND ${CLANG_TIDY_EXE}
				--config-file=${CMAKE_SOURCE_DIR}/clang_tools/.clang-tidy
				${TIDY_SOURCE_FILES}
				-p ${CMAKE_BINARY_DIR}
		)
	endif()
endif()
