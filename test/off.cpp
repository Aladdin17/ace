#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_assertion_result.hpp>
#include <alibrary/core/error.h>
#include <geometry/off.h>

static jmp_buf jmpbuf;

TEST_CASE("handle_error", "[off]")
{
    REQUIRE( 1 == 1 );
}
