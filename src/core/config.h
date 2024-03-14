/**
 * \file
 * \author Christien Alden
 * \brief Common configuration settings for the core library.
*/
#pragma once

#ifdef AL_NO_STATIC_FUNCTIONS
	#define INTERNAL
#else
	#define INTERNAL static
#endif

/**
 * \def INTERNAL
 * \brief Allows for conditional static linkage of functions based on the build requirements.
 * \remark Used to support testing of static functions in the core library.
*/
