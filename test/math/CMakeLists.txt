cmake_minimum_required( VERSION 3.8 )
target_sources(
	${PROJECT_NAME}_test
	PRIVATE
		math_test.cpp
		vec3_ext_test.cpp
		vec3_test.cpp
)
