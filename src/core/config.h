#pragma once

// conditionally compile internal functions as static based on the build configuration
// this is mainly used to expose internal functions to the test suite
#ifdef AL_NO_STATIC_FUNCTIONS
	#define INTERNAL
#else
	#define INTERNAL static
#endif
