#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_assertion_result.hpp>
#include <alibrary/core/error.h>
#include <geometry/off.h>

TEST_CASE("handle_error", "[off]") {
	jmp_buf jmpbuf;
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
