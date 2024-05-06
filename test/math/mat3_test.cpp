#include <catch2/catch_test_macros.hpp>
#include <ace/math/mat3.h>

TEST_CASE("al_mat3_identity", "[math][mat3]")
{
	mat3 m = al_mat3_identity();
	REQUIRE(m.data[0] == 1.0f);
	REQUIRE(m.data[1] == 0.0f);
	REQUIRE(m.data[2] == 0.0f);
	REQUIRE(m.data[3] == 0.0f);
	REQUIRE(m.data[4] == 1.0f);
	REQUIRE(m.data[5] == 0.0f);
	REQUIRE(m.data[6] == 0.0f);
	REQUIRE(m.data[7] == 0.0f);
	REQUIRE(m.data[8] == 1.0f);
}

TEST_CASE("transpose mat3", "[math][mat4]")
{
	mat3 m = al_mat3_identity();
	al_mat3_transpose(&m);
	REQUIRE(m.data[0] == 1.0f);
	REQUIRE(m.data[1] == 0.0f);
	REQUIRE(m.data[2] == 0.0f);
	REQUIRE(m.data[3] == 0.0f);
	REQUIRE(m.data[4] == 1.0f);
	REQUIRE(m.data[5] == 0.0f);
	REQUIRE(m.data[6] == 0.0f);
	REQUIRE(m.data[7] == 0.0f);
	REQUIRE(m.data[8] == 1.0f);
}
