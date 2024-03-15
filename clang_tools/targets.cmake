# Additional target to perform clang-format/clang-tidy run
# Requires clang-format and clang-tidy

# Get all project files
file(
	GLOB_RECURSE
	ALL_SOURCE_FILES
	${CMAKE_SOURCE_DIR}/src/*.c
)

# Get the executable path
find_program( CLANG_TIDY_EXE NAMES clang-tidy )

# add_custom_target(
# 	clang-format
# 	COMMAND /usr/bin/clang-format
# 	-style=file
# 	-i
# 	${ALL_SOURCE_FILES}
# )

add_custom_target(
	clang-tidy
	COMMAND ${CLANG_TIDY_EXE}
		${ALL_SOURCE_FILES}
		-p ${CMAKE_BINARY_DIR}
)
