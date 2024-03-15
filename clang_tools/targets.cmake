# Additional target to perform clang-format/clang-tidy run
# Requires clang-format and clang-tidy

# Get all project files
file(
	GLOB_RECURSE
	TIDY_SOURCE_FILES
	${CMAKE_SOURCE_DIR}/src/*.c
)

file(
	GLOB_RECURSE
	FORMAT_SOURCE_FILES
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/include/*.h
)

find_program( CLANG_TIDY_EXE NAMES clang-tidy )
find_program( CLANG_FORMAT_EXE NAMES clang-format )

add_custom_target(
	clang-tidy
	COMMAND ${CLANG_TIDY_EXE}
		--config-file=${CMAKE_SOURCE_DIR}/clang_tools/.clang-tidy
		${TIDY_SOURCE_FILES}
		-p ${CMAKE_BINARY_DIR}
)

add_custom_target(
	clang-format
	COMMAND ${CLANG_FORMAT_EXE}
		-i
		--style=file:${CMAKE_SOURCE_DIR}/clang_tools/.clang-format
		${FORMAT_SOURCE_FILES}
)
