#include <ace/math/math.h>
#include <ace/math/vec3.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <math.h>

//--------------------------------------------------------------------------------------------------
// float
//--------------------------------------------------------------------------------------------------

TEST_CASE( "ac_vec3 union aliases", "[ac_vec3]" ) {
    ac_vec3 v = {1.0f, 2.0f, 3.0f};
    REQUIRE(v.x == 1.0f);
    REQUIRE(v.y == 2.0f);
    REQUIRE(v.z == 3.0f);
    REQUIRE(v.r == 1.0f);
    REQUIRE(v.g == 2.0f);
    REQUIRE(v.b == 3.0f);
    REQUIRE(v.data[0] == 1.0f);
    REQUIRE(v.data[1] == 2.0f);
    REQUIRE(v.data[2] == 3.0f);
}

TEST_CASE( "ac_vec3_zero", "[ac_vec3]" ) {
    ac_vec3 zero = ac_vec3_zero();
    REQUIRE(zero.x == 0.0f);
    REQUIRE(zero.y == 0.0f);
    REQUIRE(zero.z == 0.0f);
}

TEST_CASE( "ac_vec3_nan", "[ac_vec3]" ) {
    ac_vec3 nan = ac_vec3_nan();
    REQUIRE(isnan(nan.x));
    REQUIRE(isnan(nan.y));
    REQUIRE(isnan(nan.z));
}

TEST_CASE( "ac_vec3_is_zero", "[ac_vec3]") {
    auto [input, expected] = GENERATE( Catch::Generators::table<ac_vec3, bool>({
        { ac_vec3_zero(), true },
        { {-0.0f, 0.0f, 0.0f }, true },
        { {0.0f, -0.0f, 0.0f }, true },
        { {0.0f, 0.0f, -0.0f }, true },

        { ac_vec3_nan(), false },
        { {1.0f, 0.0f, 0.0f }, false },
        { {0.0f, 1.0f, 0.0f }, false },
        { {0.0f, 0.0f, 1.0f }, false },
    }));

    CAPTURE(input.x, input.y, input.z);
    REQUIRE(ac_vec3_is_zero(&input) == expected);
}

TEST_CASE( "ac_vec3_is_nan", "[ac_vec3]" ) {
    SECTION( "nan vectors" ) {
        float x = GENERATE( Catch::Generators::values<float>({NAN, 0.0f}));
        float y = GENERATE( Catch::Generators::values<float>({NAN, 0.0f}));
        float z = GENERATE( Catch::Generators::values<float>({NAN, 0.0f}));

        ac_vec3 v = {x, y, z};
        // we only want to check the generator expressions that are not all zero
        if (!ac_vec3_is_zero(&v))
        {
            CAPTURE( x, y, z );
            REQUIRE(ac_vec3_is_nan(&v) == true);
        }
    }

    SECTION( "non-nan vectors" ) {
        float x = GENERATE( Catch::Generators::values<float>({1.0f, 0.0f}));
        float y = GENERATE( Catch::Generators::values<float>({1.0f, 0.0f}));
        float z = GENERATE( Catch::Generators::values<float>({1.0f, 0.0f}));

        ac_vec3 v = {x, y, z};
        CAPTURE( x, y, z );
        REQUIRE(ac_vec3_is_nan(&v) == false);
    }
}

TEST_CASE( "ac_vec3_is_equal", "[ac_vec3]" ) {
    SECTION( "equal vectors" ) {
        float x = GENERATE( Catch::Generators::values<float>({1.0f, 0.0f}));
        float y = GENERATE( Catch::Generators::values<float>({1.0f, 0.0f}));
        float z = GENERATE( Catch::Generators::values<float>({1.0f, 0.0f}));

        ac_vec3 a = {x, y, z};
        ac_vec3 b = {x, y, z};
        CAPTURE( x, y, z );
        REQUIRE(ac_vec3_is_equal(&a, &b) == true);
    }

    SECTION( "non-equal vectors" ) {
        auto [a, b] = GENERATE( Catch::Generators::table<ac_vec3, ac_vec3>({
            { ac_vec3_nan(), ac_vec3_nan() },
            { ac_vec3_nan(), ac_vec3_zero() },
            { ac_vec3_zero(), ac_vec3_nan() },
            { ac_vec3_zero(), {1.0f, 0.0f, 0.0f} },
            { {1.0f, 0.0f, 0.0f}, ac_vec3_zero() },
            { {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} },
            { {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f} }
        }));

        CAPTURE(a.x, a.y, a.z, b.x, b.y, b.z);
        REQUIRE(ac_vec3_is_equal(&a, &b) == false);
    }
}

TEST_CASE( "ac_vec3_add", "[ac_vec3]" ) {
    ac_vec3 a = {1.0f, 2.0f, 3.0f};
    ac_vec3 b = {4.0f, 5.0f, 6.0f};
    ac_vec3 ab_result = {5.0f, 7.0f, 9.0f};
    ac_vec3 result = ac_vec3_add(&a, &b);
    REQUIRE(ac_vec3_is_equal(&result, &ab_result) == true);
}

TEST_CASE( "ac_vec3_sub", "[ac_vec3]" ) {
    ac_vec3 a = {1.0f, 2.0f, 3.0f};
    ac_vec3 b = {4.0f, 5.0f, 6.0f};
    ac_vec3 ab_result = {-3.0f, -3.0f, -3.0f};
    ac_vec3 result = ac_vec3_sub(&a, &b);
    REQUIRE(ac_vec3_is_equal(&result, &ab_result) == true);
}

TEST_CASE( "ac_vec3_negate", "[ac_vec3]" ) {
    ac_vec3 a = {1.0f, 2.0f, 3.0f};
    ac_vec3 a_result = {-1.0f, -2.0f, -3.0f};
    ac_vec3 result = ac_vec3_negate(&a);
    REQUIRE(ac_vec3_is_equal(&result, &a_result) == true);
}

TEST_CASE( "ac_vec3_scale", "[ac_vec3]" ) {
    ac_vec3 v = {1.0f, 2.0f, 3.0f};

    SECTION( "scaling by 0" ) {
        ac_vec3 expected = {0.0f, 0.0f, 0.0f};
        ac_vec3 result = ac_vec3_scale(&v, 0.0f);
        REQUIRE(ac_vec3_is_equal(&result, &expected) == true);
    }

    SECTION( "scaling by 1" ) {
        ac_vec3 expected = {1.0f, 2.0f, 3.0f};
        ac_vec3 result = ac_vec3_scale(&v, 1.0f);
        REQUIRE(ac_vec3_is_equal(&result, &expected) == true);
    }

    SECTION( "scaling by 2" ) {
        ac_vec3 expected = {2.0f, 4.0f, 6.0f};
        ac_vec3 result = ac_vec3_scale(&v, 2.0f);
        REQUIRE(ac_vec3_is_equal(&result, &expected) == true);
    }

    SECTION( "scaling by -1" ) {
        ac_vec3 expected = {-1.0f, -2.0f, -3.0f};
        ac_vec3 result = ac_vec3_scale(&v, -1.0f);
        REQUIRE(ac_vec3_is_equal(&result, &expected) == true);
    }

    SECTION( "scaling zero vector" ) {
        ac_vec3 zv = {0.0f, 0.0f, 0.0f};
        ac_vec3 expected = {0.0f, 0.0f, 0.0f};
        ac_vec3 result = ac_vec3_scale(&zv, 2.0f);
        REQUIRE(ac_vec3_is_equal(&result, &expected) == true);
    }
}

TEST_CASE( "ac_vec3_dot", "[ac_vec3]" ) {
    SECTION( "parallel vectors" ) {
        ac_vec3 a = {1.0f, 0.0f, 0.0f};
        ac_vec3 b = {1.0f, 0.0f, 0.0f};
        float result = ac_vec3_dot(&a, &b);
        REQUIRE(result == 1.0f);

        a = {2.0f, 0.0f, 0.0f};
        b = {4.0f, 0.0f, 0.0f};
        result = ac_vec3_dot(&a, &b);
        REQUIRE(result == 8.0f);
    }

    SECTION( "perpendicular vectors" ) {
        ac_vec3 a = {1.0f, 0.0f, 0.0f};
        ac_vec3 b = {0.0f, 1.0f, 0.0f};
        float result = ac_vec3_dot(&a, &b);
        REQUIRE(result == 0.0f);
    }

    SECTION( "opposite vectors" ) {
        ac_vec3 a = {1.0f, 0.0f, 0.0f};
        ac_vec3 b = {-1.0f, 0.0f, 0.0f};
        float result = ac_vec3_dot(&a, &b);
        REQUIRE(result == -1.0f);
    }

    SECTION( "non-orthogonal vectors" ) {
        ac_vec3 a = {1.0f, 0.0f, 0.0f};
        ac_vec3 b = {1.0f, 1.0f, 0.0f};
        float result = ac_vec3_dot(&a, &b);
        REQUIRE(result == 1.0f);
    }
}

TEST_CASE( "ac_vec3_cross", "[ac_vec3]" ) {
    SECTION( "parallel vectors" ) {
        ac_vec3 a = {1.0f, 0.0f, 0.0f};
        ac_vec3 b = {1.0f, 0.0f, 0.0f};
        ac_vec3 result = ac_vec3_cross(&a, &b);
        REQUIRE(ac_vec3_is_zero(&result) == true);
    }

    SECTION( "perpendicular vectors" ) {
        ac_vec3 a = {1.0f, 0.0f, 0.0f};
        ac_vec3 b = {0.0f, 1.0f, 0.0f};
        ac_vec3 result = ac_vec3_cross(&a, &b);
        REQUIRE(result.x == 0.0f);
        REQUIRE(result.y == 0.0f);
        REQUIRE(result.z == 1.0f);
    }

    SECTION( "non-orthogonal vectors" ) {
        ac_vec3 a = {1.0f, 0.0f, 0.0f};
        ac_vec3 b = {0.0f, 1.0f, 0.0f};
        ac_vec3 result = ac_vec3_cross(&a, &b);
        REQUIRE(result.x == 0.0f);
        REQUIRE(result.y == 0.0f);
        REQUIRE(result.z == 1.0f);
    }
}

TEST_CASE( "ac_vec3_magnitude", "[ac_vec3]" ) {
    SECTION( "zero vector" ) {
        ac_vec3 zero = ac_vec3_zero();
        float result = ac_vec3_magnitude(&zero);
        REQUIRE(result == 0.0f);
    }

    SECTION( "unit vector" ) {
        ac_vec3 unit = {1.0f, 0.0f, 0.0f};
        float result = ac_vec3_magnitude(&unit);
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(1.0f, AC_EPSILON) ||
            Catch::Matchers::WithinAbs(1.0f, AC_EPSILON)
        );
    }

    SECTION( "non-unit vector" ) {
        ac_vec3 v = {1.0f, 1.0f, 1.0f};
        float result = ac_vec3_magnitude(&v);
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(1.732051f, AC_EPSILON) ||
            Catch::Matchers::WithinAbs(1.732051f, AC_EPSILON)
        );
    }
}

TEST_CASE( "ac_vec3_normalize", "[ac_vec3]" ) {
    SECTION( "nan vector" ) {
        ac_vec3 nan = ac_vec3_nan();
        ac_vec3 result = ac_vec3_normalize(&nan);
        REQUIRE(ac_vec3_is_nan(&result) == true);
    }

    SECTION( "zero vector" ) {
        ac_vec3 zero = ac_vec3_zero();
        ac_vec3 result = ac_vec3_normalize(&zero);
        REQUIRE(ac_vec3_is_nan(&result) == true);
    }

    SECTION( "unit vector" ) {
        ac_vec3 unit = {1.0f, 0.0f, 0.0f};
        ac_vec3 expected = {1.0f, 0.0f, 0.0f};
        ac_vec3 result = ac_vec3_normalize(&unit);
        REQUIRE(ac_vec3_is_equal(&result, &expected) == true);
    }

    SECTION( "non-unit vector" ) {
        ac_vec3 v = {1.0f, 1.0f, 1.0f};
        ac_vec3 expected = {0.577350f, 0.577350f, 0.577350f};
        ac_vec3 result = ac_vec3_normalize(&v);
        REQUIRE( ac_vec3_is_equal(&result, &expected) == true );
    }
}

//--------------------------------------------------------------------------------------------------
// int
//--------------------------------------------------------------------------------------------------

TEST_CASE( "ac_ivec3 union aliases", "[ac_vec3]" ) {
    ac_ivec3 v = {1, 2, 3};
    REQUIRE(v.x == 1);
    REQUIRE(v.y == 2);
    REQUIRE(v.z == 3);
    REQUIRE(v.r == 1);
    REQUIRE(v.g == 2);
    REQUIRE(v.b == 3);
    REQUIRE(v.data[0] == 1);
    REQUIRE(v.data[1] == 2);
    REQUIRE(v.data[2] == 3);
}

TEST_CASE( "ac_ivec3_zero", "[ac_vec3]" ) {
    ac_ivec3 zero = ac_ivec3_zero();
    REQUIRE(zero.x == 0);
    REQUIRE(zero.y == 0);
    REQUIRE(zero.z == 0);
}

TEST_CASE( "ac_ivec3_invalid", "[ac_vec3]" ) {
    ac_ivec3 invalid = ac_ivec3_invalid();
    REQUIRE(invalid.x == INT_INVALID);
    REQUIRE(invalid.y == INT_INVALID);
    REQUIRE(invalid.z == INT_INVALID);
}

TEST_CASE( "ac_ivec3_is_zero", "[ac_vec3]" ) {
    auto [input, expected] = GENERATE( Catch::Generators::table<ac_ivec3, bool>({
        { ac_ivec3_zero(), true },
        { {-0, 0, 0}, true },
        { {0, -0, 0}, true },
        { {0, 0, -0}, true },

        { ac_ivec3_invalid(), false },
        { {1, 0, 0}, false },
        { {0, 1, 0}, false },
        { {0, 0, 1}, false },
    }));

    CAPTURE(input.x, input.y, input.z);
    REQUIRE(ac_ivec3_is_zero(&input) == expected);
}

TEST_CASE( "ac_ivec3_is_invalid", "[ac_vec3]" ) {
    SECTION( "invalid vectors" ) {
        int x = GENERATE( Catch::Generators::values<int>({INT_INVALID, 0}));
        int y = GENERATE( Catch::Generators::values<int>({INT_INVALID, 0}));
        int z = GENERATE( Catch::Generators::values<int>({INT_INVALID, 0}));

        ac_ivec3 v = {x, y, z};
        // we only want to check the generator expressions that are not all zero
        if (!ac_ivec3_is_zero(&v))
        {
            CAPTURE( x, y, z );
            REQUIRE(ac_ivec3_is_invalid(&v) == true);
        }
    }

    SECTION( "valid vectors" ) {
        int x = GENERATE( Catch::Generators::values<int>({1, 0}));
        int y = GENERATE( Catch::Generators::values<int>({1, 0}));
        int z = GENERATE( Catch::Generators::values<int>({1, 0}));

        ac_ivec3 v = {x, y, z};
        CAPTURE( x, y, z );
        REQUIRE(ac_ivec3_is_invalid(&v) == false);
    }
}

TEST_CASE( "ac_ivec3_is_equal", "[ac_vec3]" ) {
    SECTION( "equal vectors" ) {
        int x = GENERATE( Catch::Generators::values<int>({1, 0}));
        int y = GENERATE( Catch::Generators::values<int>({1, 0}));
        int z = GENERATE( Catch::Generators::values<int>({1, 0}));

        ac_ivec3 a = {x, y, z};
        ac_ivec3 b = {x, y, z};
        CAPTURE( x, y, z );
        REQUIRE(ac_ivec3_is_equal(&a, &b) == true);
    }

    SECTION( "non-equal vectors" ) {
        auto [a, b] = GENERATE( Catch::Generators::table<ac_ivec3, ac_ivec3>({
            { ac_ivec3_invalid(), ac_ivec3_invalid() },
            { ac_ivec3_invalid(), ac_ivec3_zero() },
            { ac_ivec3_zero(), ac_ivec3_invalid() },
            { ac_ivec3_zero(), {1, 0, 0} },
            { {1, 0, 0}, ac_ivec3_zero() },
            { {1, 0, 0}, {0, 1, 0} },
            { {1, 0, 0}, {0, 0, 1} }
        }));

        CAPTURE(a.x, a.y, a.z, b.x, b.y, b.z);
        REQUIRE(ac_ivec3_is_equal(&a, &b) == false);
    }
}

TEST_CASE( "ac_ivec3_add", "[ac_vec3]" ) {
    SECTION( "invalid vectors" ) {
        ac_ivec3 invalid = ac_ivec3_invalid();
        ac_ivec3 v = { 1, 2, 3 };

        ac_ivec3 result = ac_ivec3_add(&invalid, &v);
        REQUIRE(ac_ivec3_is_invalid(&result) == true);

        result = ac_ivec3_add(&v, &invalid);
        REQUIRE(ac_ivec3_is_invalid(&result) == true);
    }

    SECTION( "valid vectors" ) {
        ac_ivec3 a = {1, 2, 3};
        ac_ivec3 b = {4, 5, 6};
        ac_ivec3 ab_result = {5, 7, 9};
        ac_ivec3 result = ac_ivec3_add(&a, &b);
        REQUIRE(ac_ivec3_is_equal(&result, &ab_result) == true);
    }
}

TEST_CASE( "ac_ivec3_sub", "[ac_vec3]" ) {
    SECTION( "invalid vectors" ) {
        ac_ivec3 invalid = ac_ivec3_invalid();
        ac_ivec3 v = { 1, 2, 3 };

        ac_ivec3 result = ac_ivec3_sub(&invalid, &v);
        REQUIRE(ac_ivec3_is_invalid(&result) == true);

        result = ac_ivec3_sub(&v, &invalid);
        REQUIRE(ac_ivec3_is_invalid(&result) == true);
    }

    SECTION( "valid vectors" ) {
        ac_ivec3 a = {1, 2, 3};
        ac_ivec3 b = {4, 5, 6};
        ac_ivec3 ab_result = {-3, -3, -3};
        ac_ivec3 result = ac_ivec3_sub(&a, &b);
        REQUIRE(ac_ivec3_is_equal(&result, &ab_result) == true);
    }
}

TEST_CASE( "ac_ivec3_negate", "[ac_vec3]" ) {
    SECTION( "invalid vector" ) {
        ac_ivec3 invalid = ac_ivec3_invalid();
        ac_ivec3 result = ac_ivec3_negate(&invalid);
        REQUIRE(ac_ivec3_is_invalid(&result) == true);
    }

    SECTION( "valid vector" ) {
        ac_ivec3 a = {1, 2, 3};
        ac_ivec3 a_result = {-1, -2, -3};
        ac_ivec3 result = ac_ivec3_negate(&a);
        REQUIRE(ac_ivec3_is_equal(&result, &a_result) == true);
    }
}

TEST_CASE( "ac_ivec3_scale", "[ac_vec3]" ) {
    SECTION( "invalid vector" ) {
        ac_ivec3 invalid = ac_ivec3_invalid();
        ac_ivec3 result = ac_ivec3_scale(&invalid, 2);
        REQUIRE(ac_ivec3_is_invalid(&result) == true);
    }

    SECTION( "zero vector" ) {
        ac_ivec3 zero = ac_ivec3_zero();
        ac_ivec3 result = ac_ivec3_scale(&zero, 2);
        REQUIRE(ac_ivec3_is_zero(&result) == true);
    }

    SECTION( "non-zero vector" ) {
        ac_ivec3 v = {1, 2, 3};
        ac_ivec3 expected = {2, 4, 6};
        ac_ivec3 result = ac_ivec3_scale(&v, 2);
        REQUIRE(ac_ivec3_is_equal(&result, &expected) == true);
    }
}

TEST_CASE( "ac_ivec3_divide", "[ac_vec3]" ) {
    SECTION( "invalid vector" ) {
        ac_ivec3 invalid = ac_ivec3_invalid();
        ac_ivec3 result = ac_ivec3_divide(&invalid, 2);
        REQUIRE(ac_ivec3_is_invalid(&result) == true);
    }

    SECTION( "zero vector" ) {
        ac_ivec3 zero = ac_ivec3_zero();
        ac_ivec3 result = ac_ivec3_divide(&zero, 2);
        REQUIRE(ac_ivec3_is_zero(&result) == true);
    }

    SECTION( "non-zero vector" ) {
        ac_ivec3 v = {2, 4, 6};
        ac_ivec3 expected = {1, 2, 3};
        ac_ivec3 result = ac_ivec3_divide(&v, 2);
        REQUIRE(ac_ivec3_is_equal(&result, &expected) == true);
    }

    SECTION( "zero scalar" ) {
        ac_ivec3 v = {2, 4, 6};
        ac_ivec3 result = ac_ivec3_divide(&v, 0);
        REQUIRE(ac_ivec3_is_invalid(&result) == true);
    }

    SECTION( "negative scalar" ) {
        ac_ivec3 v = {2, 4, 6};
        ac_ivec3 expected = {-2, -4, -6};
        ac_ivec3 result = ac_ivec3_divide(&v, -1);
        REQUIRE(ac_ivec3_is_equal(&result, &expected) == true);
    }

    SECTION( "integer division" ) {
        ac_ivec3 v = {5, 10, 15};
        ac_ivec3 expected = {2, 5, 7};
        ac_ivec3 result = ac_ivec3_divide(&v, 2);
        REQUIRE(ac_ivec3_is_equal(&result, &expected) == true);
    }
}

static int round_func_up(float value)
{
    return (int)ceilf(value);
}

static int round_func_down(float value)
{
    return (int)floorf(value);
}

static int round_func_nearest(float value)
{
    return (int)roundf(value);
}

TEST_CASE( "ac_ivec3_divide_ext", "[ac_vec3]" ) {
    SECTION( "invalid vector" ) {
        ac_ivec3 invalid = ac_ivec3_invalid();
        ac_ivec3 result = ac_ivec3_divide_ext(&invalid, 2, round_func_nearest);
        REQUIRE(ac_ivec3_is_invalid(&result) == true);
    }

    SECTION( "zero vector" ) {
        ac_ivec3 zero = ac_ivec3_zero();
        ac_ivec3 result = ac_ivec3_divide_ext(&zero, 2, round_func_nearest);
        REQUIRE(ac_ivec3_is_zero(&result) == true);
    }

    SECTION( "zero scalar" ) {
        ac_ivec3 v = {2, 4, 6};
        ac_ivec3 result = ac_ivec3_divide_ext(&v, 0, round_func_nearest);
        REQUIRE(ac_ivec3_is_invalid(&result) == true);
    }

    SECTION( "round up" ) {
        ac_ivec3 v = {5, 10, 14};
        ac_ivec3 expected = {2, 4, 5};
        ac_ivec3 result = ac_ivec3_divide_ext(&v, 3, round_func_up);
        REQUIRE(ac_ivec3_is_equal(&result, &expected) == true);
    }

    SECTION( "round down" ) {
        ac_ivec3 v = {5, 10, 14};
        ac_ivec3 expected = {1, 3, 4};
        ac_ivec3 result = ac_ivec3_divide_ext(&v, 3, round_func_down);
        REQUIRE(ac_ivec3_is_equal(&result, &expected) == true);
    }

    SECTION( "round nearest" ) {
        ac_ivec3 v = {5, 10, 14};
        ac_ivec3 expected = {2, 3, 5};
        ac_ivec3 result = ac_ivec3_divide_ext(&v, 3, round_func_nearest);
        REQUIRE(ac_ivec3_is_equal(&result, &expected) == true);
    }
}

TEST_CASE( "ac_ivec3_dot", "[ac_vec3]" ) {
    SECTION( "invalid vector" ) {
        ac_ivec3 invalid = ac_ivec3_invalid();
        ac_ivec3 v = {1, 0, 0};

        int result = ac_ivec3_dot(&invalid, &v);
        REQUIRE( result == INT_INVALID);

        result = ac_ivec3_dot(&v, &invalid);
        REQUIRE( result == INT_INVALID);
    }

    SECTION( "parallel vectors" ) {
        ac_ivec3 a = {1, 0, 0};
        ac_ivec3 b = {1, 0, 0};
        int result = ac_ivec3_dot(&a, &b);
        REQUIRE(result == 1);

        a = {2, 0, 0};
        b = {4, 0, 0};
        result = ac_ivec3_dot(&a, &b);
        REQUIRE(result == 8);
    }

    SECTION( "perpendicular vectors" ) {
        ac_ivec3 a = {1, 0, 0};
        ac_ivec3 b = {0, 1, 0};
        int result = ac_ivec3_dot(&a, &b);
        REQUIRE(result == 0);
    }

    SECTION( "opposite vectors" ) {
        ac_ivec3 a = {1, 0, 0};
        ac_ivec3 b = {-1, 0, 0};
        int result = ac_ivec3_dot(&a, &b);
        REQUIRE(result == -1);
    }

    SECTION( "non-orthogonal vectors" ) {
        ac_ivec3 a = {1, 0, 0};
        ac_ivec3 b = {1, 1, 0};
        int result = ac_ivec3_dot(&a, &b);
        REQUIRE(result == 1);
    }
}

TEST_CASE( "ac_ivec3_cross", "[ac_vec3]" ) {
    SECTION( "invalid vector" ) {
        ac_ivec3 invalid = ac_ivec3_invalid();
        ac_ivec3 v = {1, 0, 0};

        ac_ivec3 result = ac_ivec3_cross(&invalid, &v);
        REQUIRE(ac_ivec3_is_invalid(&result) == true);

        result = ac_ivec3_cross(&v, &invalid);
        REQUIRE(ac_ivec3_is_invalid(&result) == true);
    }

    SECTION( "parallel vectors" ) {
        ac_ivec3 a = {1, 0, 0};
        ac_ivec3 b = {1, 0, 0};
        ac_ivec3 result = ac_ivec3_cross(&a, &b);
        REQUIRE(ac_ivec3_is_zero(&result) == true);
    }

    SECTION( "perpendicular vectors" ) {
        ac_ivec3 a = {1, 0, 0};
        ac_ivec3 b = {0, 1, 0};
        ac_ivec3 result = ac_ivec3_cross(&a, &b);
        REQUIRE(result.x == 0);
        REQUIRE(result.y == 0);
        REQUIRE(result.z == 1);
    }

    SECTION( "non-orthogonal vectors" ) {
        ac_ivec3 a = {1, 0, 0};
        ac_ivec3 b = {0, 1, 0};
        ac_ivec3 result = ac_ivec3_cross(&a, &b);
        REQUIRE(result.x == 0);
        REQUIRE(result.y == 0);
        REQUIRE(result.z == 1);
    }
}
