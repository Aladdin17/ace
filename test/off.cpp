#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_assertion_result.hpp>
#include <alibrary/core/error.h>
#include <geometry/off.h>

static jmp_buf jmpbuf;

TEST_CASE("handle_error", "[off]")
{
    REQUIRE( 1 == 0 );
	// interaction between '_setjmp' and C++ object destruction is non-portable
	// but we need to test within this context
	#pragma warning(disable: 4611)
	if (setjmp(jmpbuf) == 1)
	{
		const char* msg = GetGlobalErrorMessage();
		REQUIRE(strcmp(msg, "test") == 0);
	}
	else
	{
		HandleError(jmpbuf, "test");
	}
}
