cmake_minimum_required( VERSION 3.8 )
target_sources(
	${PROJECT_NAME}
	PRIVATE
		config.h # defines the configuration of the library
		error.c
		string.c
)
