cmake_minimum_required( VERSION 3.9 )

project ( ace_demo )

# file(GLOB DEMO_SOURCES
#     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
# )

add_executable(${PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
	${CMAKE_CURRENT_SOURCE_DIR}/app.c
    # ${DEMO_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE ace freeglut_static)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        C_STANDARD 11
        C_STANDARD_REQUIRED ON
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

# add src as an include dir too.
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
