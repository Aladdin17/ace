cmake_minimum_required( VERSION 3.8 )
target_sources(
	${PROJECT_NAME}
	PRIVATE
		mat2.c
		math.c
		vec3.c
		vec3_ext.c
)
