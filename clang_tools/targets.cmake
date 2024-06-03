# Additional target to perform clang-format/clang-tidy run
# Requires clang-format and clang-tidy

#---------------------------------------------------------------------------------------------------
# Clang-Format
#---------------------------------------------------------------------------------------------------
find_program( CLANG_FORMAT_EXE NAMES clang-format )
if( NOT CLANG_FORMAT_EXE )
	message( STATUS "Clang-format executable not found" )
else()
	execute_process(
		COMMAND ${CLANG_FORMAT_EXE} --version
		OUTPUT_VARIABLE CLANG_FORMAT_VERSION_OUTPUT
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)

	set( CLANG_FORMAT_MIN_VERSION "18.0.0" )
	string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+" CLANG_FORMAT_VERSION ${CLANG_FORMAT_VERSION_OUTPUT})
	if( CLANG_FORMAT_VERSION VERSION_LESS ${CLANG_FORMAT_MIN_VERSION} )
		message( STATUS "Clang-format version must be at least ${CLANG_FORMAT_MIN_VERSION}, found ${CLANG_FORMAT_VERSION}" )
	else()
		message( STATUS "Clang-format configured - Version ${CLANG_FORMAT_VERSION}" )
		file(
			GLOB_RECURSE
			FORMAT_SOURCE_FILES
			${CMAKE_SOURCE_DIR}/src/*.c
			${CMAKE_SOURCE_DIR}/src/*.h
			${CMAKE_SOURCE_DIR}/include/*.h
			${CMAKE_SOURCE_DIR}/demo/*.c
			${CMAKE_SOURCE_DIR}/demo/*.h
		)

		add_custom_target(
			clang-format
			COMMAND ${CLANG_FORMAT_EXE}
				-i
				--style=file:${CMAKE_SOURCE_DIR}/clang_tools/v18/.clang-format
				${FORMAT_SOURCE_FILES}
			COMMENT
				"Running clang-format"
		)

		add_custom_target(
			check-clang-format
			COMMAND ${CLANG_FORMAT_EXE}
				--dry-run
				--Werror
				--style=file:${CMAKE_SOURCE_DIR}/clang_tools/v18/.clang-format
				${FORMAT_SOURCE_FILES}
			COMMENT
				"Checking code style with clang-format"
		)
	endif()
endif()


#---------------------------------------------------------------------------------------------------
# Clang-Tidy
#---------------------------------------------------------------------------------------------------
find_program( CLANG_TIDY_EXE NAMES clang-tidy )
if( NOT CLANG_TIDY_EXE )
	message( STATUS "Clang-tidy executable not found" )
else()
	set( CLANG_TIDY_COMPATIBLE_GENERATORS "Ninja" "Unix Makefiles" "MinGW Makefiles" )
	list( FIND CLANG_TIDY_COMPATIBLE_GENERATORS "${CMAKE_GENERATOR}" _index )
	if( ${_index} EQUAL -1 )
		message( STATUS "Clang-tidy is not compatible with the current generator: ${CMAKE_GENERATOR}" )
	else()
		message( STATUS "Clang-tidy configured - Version " )
		file(
			GLOB_RECURSE
			TIDY_SOURCE_FILES
			${CMAKE_SOURCE_DIR}/src/*.c
			${CMAKE_SOURCE_DIR}/demo/*.c
		)

		add_custom_target(
			clang-tidy
			COMMAND ${CLANG_TIDY_EXE}
				--config-file=${CMAKE_SOURCE_DIR}/clang_tools/v14/.clang-tidy
				${TIDY_SOURCE_FILES}
				-p ${CMAKE_BINARY_DIR}
			COMMENT
				"Running clang-tidy"
		)

		add_custom_target(
			check-clang-tidy
			COMMAND ${CLANG_TIDY_EXE}
				--config-file=${CMAKE_SOURCE_DIR}/clang_tools/v14/.clang-tidy
				--warnings-as-errors='*'
				${TIDY_SOURCE_FILES}
				-p ${CMAKE_BINARY_DIR}
			COMMENT
				"Checking code with clang-tidy"
		)
	endif()
endif()
