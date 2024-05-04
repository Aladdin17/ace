cmake_minimum_required( VERSION 3.8 )
set( FREEGLUT_BUILD_DEMOS OFF )
set( FREEGLUT_BUILD_SHARED_LIBS OFF )
add_subdirectory( freeglut )
# Need to do this due to the freeglut_static cmake target not exposing their include directory
target_include_directories( ${PROJECT_NAME} PUBLIC SYSTEM freeglut/include )
