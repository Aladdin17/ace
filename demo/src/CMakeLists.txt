cmake_minimum_required( VERSION 3.8 )
target_sources(
	ace_demo
	PRIVATE
		app.c
		app.h
		init.c
		init.h
		render.c
		render.h
		timer.h
		types.h
)
