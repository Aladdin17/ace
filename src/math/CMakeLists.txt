cmake_minimum_required( VERSION 3.8 )
target_sources(
	${PROJECT_NAME}
	PRIVATE
		math.c
		vec2_ext.c
		vec2.c
		vec3_ext.c
		vec3.c
)
