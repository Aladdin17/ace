#include <ace/math/math.h>
#include <ace/math/vec4.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <math.h>

//--------------------------------------------------------------------------------------------------
// float
//--------------------------------------------------------------------------------------------------

TEST_CASE( "ac_vec4 union aliases", "[ac_vec4]" ) {
    ac_vec4 v = {1.0f, 2.0f, 3.0f, 4.0f};
    REQUIRE(v.x == 1.0f);
    REQUIRE(v.y == 2.0f);
    REQUIRE(v.z == 3.0f);
    REQUIRE(v.w == 4.0f);
    REQUIRE(v.r == 1.0f);
    REQUIRE(v.g == 2.0f);
    REQUIRE(v.b == 3.0f);
    REQUIRE(v.a == 4.0f);
    REQUIRE(v.data[0] == 1.0f);
    REQUIRE(v.data[1] == 2.0f);
    REQUIRE(v.data[2] == 3.0f);
    REQUIRE(v.data[3] == 4.0f);
}

TEST_CASE( "ac_vec4_zero", "[ac_vec4]" ) {
    ac_vec4 zero = ac_vec4_zero();
    REQUIRE(zero.x == 0.0f);
    REQUIRE(zero.y == 0.0f);
    REQUIRE(zero.z == 0.0f);
    REQUIRE(zero.w == 0.0f);
}

TEST_CASE( "ac_vec4_nan", "[ac_vec4]" ) {
    ac_vec4 nan = ac_vec4_nan();
    REQUIRE(isnan(nan.x));
    REQUIRE(isnan(nan.y));
    REQUIRE(isnan(nan.z));
    REQUIRE(isnan(nan.w));
}

TEST_CASE( "ac_vec4_is_zero", "[ac_vec4]") {
    auto [input, expected] = GENERATE( Catch::Generators::table<ac_vec4, bool>({
        { ac_vec4_zero(), true },
        { {-0.0f, 0.0f, 0.0f, 0.0f}, true },
        { {0.0f, -0.0f, 0.0f, 0.0f}, true },
        { {0.0f, 0.0f, -0.0f, 0.0f}, true },
        { {0.0f, 0.0f, 0.0f, -0.0f}, true },

        { ac_vec4_nan(), false },
        { {1.0f, 0.0f, 0.0f, 0.0f}, false },
        { {0.0f, 1.0f, 0.0f, 0.0f}, false },
        { {0.0f, 0.0f, 1.0f, 0.0f}, false },
        { {0.0f, 0.0f, 0.0f, 1.0f}, false },
    }));

    CAPTURE(input.x, input.y, input.z, input.w);
    REQUIRE(ac_vec4_is_zero(&input) == expected);
}

TEST_CASE( "ac_vec4_is_nan", "[ac_vec4]" ) {
    SECTION( "nan vectors" ) {
        float x = GENERATE( Catch::Generators::values<float>({NAN, 0.0f}));
        float y = GENERATE( Catch::Generators::values<float>({NAN, 0.0f}));
        float z = GENERATE( Catch::Generators::values<float>({NAN, 0.0f}));
        float w = GENERATE( Catch::Generators::values<float>({NAN, 0.0f}));

        ac_vec4 v = {x, y, z, w};
        // we only want to check the generator expressions that are not all zero
        if (!ac_vec4_is_zero(&v))
        {
            CAPTURE( x, y, z, w );
            REQUIRE(ac_vec4_is_nan(&v) == true);
        }
    }

    SECTION( "non-nan vectors" ) {
        float x = GENERATE( Catch::Generators::values<float>({1.0f, 0.0f}));
        float y = GENERATE( Catch::Generators::values<float>({1.0f, 0.0f}));
        float z = GENERATE( Catch::Generators::values<float>({1.0f, 0.0f}));
        float w = GENERATE( Catch::Generators::values<float>({1.0f, 0.0f}));

        ac_vec4 v = {x, y, z, w};
        CAPTURE( x, y, z, w );
        REQUIRE(ac_vec4_is_nan(&v) == false);
    }
}

TEST_CASE( "ac_vec4_is_equal", "[ac_vec4]" ) {
    SECTION( "equal vectors" ) {
        float x = GENERATE( Catch::Generators::values<float>({1.0f, 0.0f}));
        float y = GENERATE( Catch::Generators::values<float>({1.0f, 0.0f}));
        float z = GENERATE( Catch::Generators::values<float>({1.0f, 0.0f}));
        float w = GENERATE( Catch::Generators::values<float>({1.0f, 0.0f}));

        ac_vec4 a = {x, y, z, w};
        ac_vec4 b = {x, y, z, w};
        CAPTURE( x, y, z, w );
        REQUIRE(ac_vec4_is_equal(&a, &b) == true);
    }

    SECTION( "non-equal vectors" ) {
        auto [a, b] = GENERATE( Catch::Generators::table<ac_vec4, ac_vec4>({
            { ac_vec4_nan(), ac_vec4_nan() },
            { ac_vec4_nan(), ac_vec4_zero() },
            { ac_vec4_zero(), ac_vec4_nan() },
            { ac_vec4_zero(), {1.0f, 0.0f, 0.0f, 0.0f} },
            { {1.0f, 0.0f, 0.0f, 0.0f}, ac_vec4_zero() },
            { {1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f} },
            { {1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f} },
            { {1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f} }
        }));

        CAPTURE(a.x, a.y, a.z, a.w, b.x, b.y, b.z, b.w);
        REQUIRE(ac_vec4_is_equal(&a, &b) == false);
    }
}

TEST_CASE( "ac_vec4_add", "[ac_vec4]" ) {
    ac_vec4 a = {1.0f, 2.0f, 3.0f, 1.0f};
    ac_vec4 b = {4.0f, 5.0f, 6.0f, 1.0f};
    ac_vec4 ab_result = {5.0f, 7.0f, 9.0f, 2.0f};
    ac_vec4 result = ac_vec4_add(&a, &b);
    REQUIRE(ac_vec4_is_equal(&result, &ab_result) == true);
}

TEST_CASE( "ac_vec4_sub", "[ac_vec4]" ) {
    ac_vec4 a = {1.0f, 2.0f, 3.0f, 1.0f};
    ac_vec4 b = {4.0f, 5.0f, 6.0f, 1.0f};
    ac_vec4 ab_result = {-3.0f, -3.0f, -3.0f, 0.0f};
    ac_vec4 result = ac_vec4_sub(&a, &b);
    REQUIRE(ac_vec4_is_equal(&result, &ab_result) == true);
}

TEST_CASE( "ac_vec4_negate", "[ac_vec4]" ) {
    ac_vec4 a = {1.0f, 2.0f, 3.0f, 1.0f};
    ac_vec4 a_result = {-1.0f, -2.0f, -3.0f, -1.0f};
    ac_vec4 result = ac_vec4_negate(&a);
    REQUIRE(ac_vec4_is_equal(&result, &a_result) == true);
}

TEST_CASE( "ac_vec4_scale", "[ac_vec4]" ) {
    ac_vec4 v = {1.0f, 2.0f, 3.0f, 1.0f};

    SECTION( "scaling by 0" ) {
        ac_vec4 expected = {0.0f, 0.0f, 0.0f, 0.0f};
        ac_vec4 result = ac_vec4_scale(&v, 0.0f);
        REQUIRE(ac_vec4_is_equal(&result, &expected) == true);
    }

    SECTION( "scaling by 1" ) {
        ac_vec4 expected = {1.0f, 2.0f, 3.0f, 1.0f};
        ac_vec4 result = ac_vec4_scale(&v, 1.0f);
        REQUIRE(ac_vec4_is_equal(&result, &expected) == true);
    }

    SECTION( "scaling by 2" ) {
        ac_vec4 expected = {2.0f, 4.0f, 6.0f, 2.0f};
        ac_vec4 result = ac_vec4_scale(&v, 2.0f);
        REQUIRE(ac_vec4_is_equal(&result, &expected) == true);
    }

    SECTION( "scaling by -1" ) {
        ac_vec4 expected = {-1.0f, -2.0f, -3.0f, -1.0f};
        ac_vec4 result = ac_vec4_scale(&v, -1.0f);
        REQUIRE(ac_vec4_is_equal(&result, &expected) == true);
    }

    SECTION( "scaling zero vector" ) {
        ac_vec4 zv = {0.0f, 0.0f, 0.0f, 0.0f};
        ac_vec4 expected = {0.0f, 0.0f, 0.0f, 0.0f};
        ac_vec4 result = ac_vec4_scale(&zv, 2.0f);
        REQUIRE(ac_vec4_is_equal(&result, &expected) == true);
    }
}

TEST_CASE( "ac_vec4_dot", "[ac_vec4]" ) {
    SECTION( "parallel vectors" ) {
        ac_vec4 a = {1.0f, 0.0f, 0.0f, 0.0f};
        ac_vec4 b = {1.0f, 0.0f, 0.0f, 0.0f};
        float result = ac_vec4_dot(&a, &b);
        REQUIRE(result == 1.0f);

        a = {2.0f, 0.0f, 0.0f, 0.0f};
        b = {4.0f, 0.0f, 0.0f, 0.0f};
        result = ac_vec4_dot(&a, &b);
        REQUIRE(result == 8.0f);
    }

    SECTION( "perpendicular vectors" ) {
        ac_vec4 a = {1.0f, 0.0f, 0.0f, 0.0f};
        ac_vec4 b = {0.0f, 1.0f, 0.0f, 0.0f};
        float result = ac_vec4_dot(&a, &b);
        REQUIRE(result == 0.0f);
    }

    SECTION( "opposite vectors" ) {
        ac_vec4 a = {1.0f, 0.0f, 0.0f, 0.0f};
        ac_vec4 b = {-1.0f, 0.0f, 0.0f, 0.0f};
        float result = ac_vec4_dot(&a, &b);
        REQUIRE(result == -1.0f);
    }

    SECTION( "non-orthogonal vectors" ) {
        ac_vec4 a = {1.0f, 0.0f, 0.0f, 0.0f};
        ac_vec4 b = {1.0f, 1.0f, 0.0f, 0.0f};
        float result = ac_vec4_dot(&a, &b);
        REQUIRE(result == 1.0f);
    }
}

TEST_CASE( "ac_vec4_magnitude", "[ac_vec4]" ) {
    SECTION( "zero vector" ) {
        ac_vec4 zero = ac_vec4_zero();
        float result = ac_vec4_magnitude(&zero);
        REQUIRE(result == 0.0f);
    }

    SECTION( "unit vector" ) {
        ac_vec4 unit = {1.0f, 0.0f, 0.0f, 0.0f};
        float result = ac_vec4_magnitude(&unit);
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(1.0f, AC_EPSILON) ||
            Catch::Matchers::WithinAbs(1.0f, AC_EPSILON)
        );
    }

    SECTION( "non-unit vector" ) {
        ac_vec4 v = {1.0f, 1.0f, 1.0f, 1.0f};
        float result = ac_vec4_magnitude(&v);
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(2.0f, AC_EPSILON) ||
            Catch::Matchers::WithinAbs(2.0f, AC_EPSILON)
        );
    }
}

TEST_CASE( "ac_vec4_normalize", "[ac_vec4]" ) {
    SECTION( "nan vector" ) {
        ac_vec4 nan = ac_vec4_nan();
        ac_vec4 result = ac_vec4_normalize(&nan);
        REQUIRE(ac_vec4_is_nan(&result) == true);
    }

    SECTION( "zero vector" ) {
        ac_vec4 zero = ac_vec4_zero();
        ac_vec4 result = ac_vec4_normalize(&zero);
        REQUIRE(ac_vec4_is_nan(&result) == true);
    }

    SECTION( "unit vector" ) {
        ac_vec4 unit = {1.0f, 0.0f, 0.0f, 0.0f};
        ac_vec4 expected = {1.0f, 0.0f, 0.0f, 0.0f};
        ac_vec4 result = ac_vec4_normalize(&unit);
        REQUIRE(ac_vec4_is_equal(&result, &expected) == true);
    }

    SECTION( "non-unit vector" ) {
        ac_vec4 v = {1.0f, 1.0f, 1.0f, 1.0f};
        ac_vec4 expected = {0.5f, 0.5f, 0.5f, 0.5f};
        ac_vec4 result = ac_vec4_normalize(&v);
        REQUIRE(ac_vec4_is_equal(&result, &expected) == true);
    }
}

//--------------------------------------------------------------------------------------------------
// int
//--------------------------------------------------------------------------------------------------

TEST_CASE( "ac_ivec4 union aliases", "[ac_vec4]" ) {
    ac_ivec4 v = {1, 2, 3, 4};
    REQUIRE(v.x == 1);
    REQUIRE(v.y == 2);
    REQUIRE(v.z == 3);
    REQUIRE(v.w == 4);
    REQUIRE(v.r == 1);
    REQUIRE(v.g == 2);
    REQUIRE(v.b == 3);
    REQUIRE(v.a == 4);
    REQUIRE(v.data[0] == 1);
    REQUIRE(v.data[1] == 2);
    REQUIRE(v.data[2] == 3);
    REQUIRE(v.data[3] == 4);
}

TEST_CASE( "ac_ivec4_zero", "[ac_vec4]" ) {
    ac_ivec4 zero = ac_ivec4_zero();
    REQUIRE(zero.x == 0);
    REQUIRE(zero.y == 0);
    REQUIRE(zero.z == 0);
    REQUIRE(zero.w == 0);
}

TEST_CASE( "ac_ivec4_invalid", "[ac_vec4]" ) {
    ac_ivec4 invalid = ac_ivec4_invalid();
    REQUIRE(invalid.x == INT_INVALID);
    REQUIRE(invalid.y == INT_INVALID);
    REQUIRE(invalid.z == INT_INVALID);
    REQUIRE(invalid.w == INT_INVALID);
}

TEST_CASE( "ac_ivec4_is_zero", "[ac_vec4]" ) {
    auto [input, expected] = GENERATE( Catch::Generators::table<ac_ivec4, bool>({
        { {0, 0, 0, 0}, true },
        { {-0, 0, 0, 0}, true },
        { {0, -0, 0, 0}, true },
        { {0, 0, -0, 0}, true },
        { {0, 0, 0, -0}, true },

        { {INT_INVALID, INT_INVALID, INT_INVALID, INT_INVALID}, false },
        { {1, 0, 0, 0}, false },
        { {0, 1, 0, 0}, false },
        { {0, 0, 1, 0}, false },
        { {0, 0, 0, 1}, false },
    }));

    CAPTURE(input.x, input.y, input.z, input.w);
    REQUIRE(ac_ivec4_is_zero(&input) == expected);
}

TEST_CASE( "ac_ivec4_is_invalid", "[ac_vec4]" ) {
    SECTION( "invalid vectors" ) {
        int x = GENERATE( Catch::Generators::values<int>({INT_INVALID, 0}));
        int y = GENERATE( Catch::Generators::values<int>({INT_INVALID, 0}));
        int z = GENERATE( Catch::Generators::values<int>({INT_INVALID, 0}));
        int w = GENERATE( Catch::Generators::values<int>({INT_INVALID, 0}));

        ac_ivec4 v = {x, y, z, w};
        // we only want to check the generator expressions that are not all zero
        if (!ac_ivec4_is_zero(&v))
        {
            CAPTURE( x, y, z, w );
            REQUIRE(ac_ivec4_is_invalid(&v) == true);
        }
    }

    SECTION( "valid vectors" ) {
        int x = GENERATE( Catch::Generators::values<int>({1, 0}));
        int y = GENERATE( Catch::Generators::values<int>({1, 0}));
        int z = GENERATE( Catch::Generators::values<int>({1, 0}));
        int w = GENERATE( Catch::Generators::values<int>({1, 0}));

        ac_ivec4 v = {x, y, z, w};
        CAPTURE( x, y, z, w );
        REQUIRE(ac_ivec4_is_invalid(&v) == false);
    }
}

TEST_CASE( "ac_ivec4_is_equal", "[ac_vec4]" ) {
    SECTION( "equal vectors" ) {
        int x = GENERATE( Catch::Generators::values<int>({1, 0}));
        int y = GENERATE( Catch::Generators::values<int>({1, 0}));
        int z = GENERATE( Catch::Generators::values<int>({1, 0}));
        int w = GENERATE( Catch::Generators::values<int>({1, 0}));

        ac_ivec4 a = {x, y, z, w};
        ac_ivec4 b = {x, y, z, w};
        CAPTURE( x, y, z, w );
        REQUIRE(ac_ivec4_is_equal(&a, &b) == true);
    }

    SECTION( "non-equal vectors" ) {
        auto [a, b] = GENERATE( Catch::Generators::table<ac_ivec4, ac_ivec4>({
            { {INT_INVALID, INT_INVALID, INT_INVALID, INT_INVALID}, {INT_INVALID, INT_INVALID, INT_INVALID, INT_INVALID} },
            { {INT_INVALID, INT_INVALID, INT_INVALID, INT_INVALID}, {0, 0, 0, 0} },
            { {0, 0, 0, 0}, {INT_INVALID, INT_INVALID, INT_INVALID, INT_INVALID} },
            { {0, 0, 0, 0}, {1, 0, 0, 0} },
            { {1, 0, 0, 0}, {0, 0, 0, 0} },
            { {1, 0, 0, 0}, {0, 1, 0, 0} },
            { {1, 0, 0, 0}, {0, 0, 1, 0} },
            { {1, 0, 0, 0}, {0, 0, 0, 1} }
        }));

        CAPTURE(a.x, a.y, a.z, a.w, b.x, b.y, b.z, b.w);
        REQUIRE(ac_ivec4_is_equal(&a, &b) == false);
    }
}

TEST_CASE( "ac_ivec4_add", "[ac_vec4]" ) {
    SECTION( "invalid vectors" ) {
        ac_ivec4 invalid = ac_ivec4_invalid();
        ac_ivec4 v = { 1, 2, 3, 4 };

        ac_ivec4 result = ac_ivec4_add(&invalid, &v);
        REQUIRE(ac_ivec4_is_invalid(&result) == true);

        result = ac_ivec4_add(&v, &invalid);
        REQUIRE(ac_ivec4_is_invalid(&result) == true);
    }

    SECTION( "valid vectors" ) {
        ac_ivec4 a = {1, 2, 3, 4};
        ac_ivec4 b = {4, 5, 6, 7};
        ac_ivec4 ab_result = {5, 7, 9, 11};
        ac_ivec4 result = ac_ivec4_add(&a, &b);
        REQUIRE(ac_ivec4_is_equal(&result, &ab_result) == true);
    }
}

TEST_CASE( "ac_ivec4_sub", "[ac_vec4]" ) {
    SECTION( "invalid vectors" ) {
        ac_ivec4 invalid = ac_ivec4_invalid();
        ac_ivec4 v = { 1, 2, 3, 4 };

        ac_ivec4 result = ac_ivec4_sub(&invalid, &v);
        REQUIRE(ac_ivec4_is_invalid(&result) == true);

        result = ac_ivec4_sub(&v, &invalid);
        REQUIRE(ac_ivec4_is_invalid(&result) == true);
    }

    SECTION( "valid vectors" ) {
        ac_ivec4 a = {1, 2, 3, 4};
        ac_ivec4 b = {4, 5, 6, 7};
        ac_ivec4 ab_result = {-3, -3, -3, -3};
        ac_ivec4 result = ac_ivec4_sub(&a, &b);
        REQUIRE(ac_ivec4_is_equal(&result, &ab_result) == true);
    }
}

TEST_CASE( "ac_ivec4_negate", "[ac_vec4]" ) {
    SECTION( "invalid vector" ) {
        ac_ivec4 invalid = ac_ivec4_invalid();
        ac_ivec4 result = ac_ivec4_negate(&invalid);
        REQUIRE(ac_ivec4_is_invalid(&result) == true);
    }

    SECTION( "valid vector" ) {
        ac_ivec4 a = {1, 2, 3, 4};
        ac_ivec4 a_result = {-1, -2, -3, -4};
        ac_ivec4 result = ac_ivec4_negate(&a);
        REQUIRE(ac_ivec4_is_equal(&result, &a_result) == true);
    }
}

TEST_CASE( "ac_ivec4_scale", "[ac_vec4]" ) {
    SECTION( "invalid vector" ) {
        ac_ivec4 invalid = ac_ivec4_invalid();
        ac_ivec4 result = ac_ivec4_scale(&invalid, 2);
        REQUIRE(ac_ivec4_is_invalid(&result) == true);
    }

    SECTION( "zero vector" ) {
        ac_ivec4 zero = ac_ivec4_zero();
        ac_ivec4 result = ac_ivec4_scale(&zero, 2);
        REQUIRE(ac_ivec4_is_zero(&result) == true);
    }

    SECTION( "non-zero vector" ) {
        ac_ivec4 v = {1, 2, 3, 4};
        ac_ivec4 expected = {2, 4, 6, 8};
        ac_ivec4 result = ac_ivec4_scale(&v, 2);
        REQUIRE(ac_ivec4_is_equal(&result, &expected) == true);
    }
}

TEST_CASE( "ac_ivec4_divide", "[ac_vec4]" ) {
    SECTION( "invalid vector" ) {
        ac_ivec4 invalid = ac_ivec4_invalid();
        ac_ivec4 result = ac_ivec4_divide(&invalid, 2);
        REQUIRE(ac_ivec4_is_invalid(&result) == true);
    }

    SECTION( "zero vector" ) {
        ac_ivec4 zero = ac_ivec4_zero();
        ac_ivec4 result = ac_ivec4_divide(&zero, 2);
        REQUIRE(ac_ivec4_is_zero(&result) == true);
    }

    SECTION( "non-zero vector" ) {
        ac_ivec4 v = {2, 4, 6, 8};
        ac_ivec4 expected = {1, 2, 3, 4};
        ac_ivec4 result = ac_ivec4_divide(&v, 2);
        REQUIRE(ac_ivec4_is_equal(&result, &expected) == true);
    }

    SECTION( "zero scalar" ) {
        ac_ivec4 v = {2, 4, 6, 8};
        ac_ivec4 result = ac_ivec4_divide(&v, 0);
        REQUIRE(ac_ivec4_is_invalid(&result) == true);
    }

    SECTION( "negative scalar" ) {
        ac_ivec4 v = {2, 4, 6, 8};
        ac_ivec4 expected = {-2, -4, -6, -8};
        ac_ivec4 result = ac_ivec4_divide(&v, -1);
        REQUIRE(ac_ivec4_is_equal(&result, &expected) == true);
    }

    SECTION( "integer division" ) {
        ac_ivec4 v = {5, 10, 15, 20};
        ac_ivec4 expected = {2, 5, 7, 10};
        ac_ivec4 result = ac_ivec4_divide(&v, 2);
        REQUIRE(ac_ivec4_is_equal(&result, &expected) == true);
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

TEST_CASE( "ac_ivec4_divide_ext", "[ac_vec4]" ) {
    SECTION( "invalid vector" ) {
        ac_ivec4 invalid = ac_ivec4_invalid();
        ac_ivec4 result = ac_ivec4_divide_ext(&invalid, 2, round_func_nearest);
        REQUIRE(ac_ivec4_is_invalid(&result) == true);
    }

    SECTION( "zero vector" ) {
        ac_ivec4 zero = ac_ivec4_zero();
        ac_ivec4 result = ac_ivec4_divide_ext(&zero, 2, round_func_nearest);
        REQUIRE(ac_ivec4_is_zero(&result) == true);
    }

    SECTION( "zero scalar" ) {
        ac_ivec4 v = {2, 4, 6, 8};
        ac_ivec4 result = ac_ivec4_divide_ext(&v, 0, round_func_nearest);
        REQUIRE(ac_ivec4_is_invalid(&result) == true);
    }

    SECTION( "round up" ) {
        ac_ivec4 v = {5, 10, 14, 18};
        ac_ivec4 expected = {2, 4, 5, 6};
        ac_ivec4 result = ac_ivec4_divide_ext(&v, 3, round_func_up);
        REQUIRE(ac_ivec4_is_equal(&result, &expected) == true);
    }

    SECTION( "round down" ) {
        ac_ivec4 v = {5, 10, 14, 18};
        ac_ivec4 expected = {1, 3, 4, 6};
        ac_ivec4 result = ac_ivec4_divide_ext(&v, 3, round_func_down);
        REQUIRE(ac_ivec4_is_equal(&result, &expected) == true);
    }

    SECTION( "round nearest" ) {
        ac_ivec4 v = {5, 10, 14, 18};
        ac_ivec4 expected = {2, 3, 5, 6};
        ac_ivec4 result = ac_ivec4_divide_ext(&v, 3, round_func_nearest);
        REQUIRE(ac_ivec4_is_equal(&result, &expected) == true);
    }
}

TEST_CASE( "ac_ivec4_dot", "[ac_vec4]" ) {
    SECTION( "invalid vector" ) {
        ac_ivec4 invalid = ac_ivec4_invalid();
        ac_ivec4 v = {1, 0, 0, 0};

        int result = ac_ivec4_dot(&invalid, &v);
        REQUIRE( result == INT_INVALID);

        result = ac_ivec4_dot(&v, &invalid);
        REQUIRE( result == INT_INVALID);
    }

    SECTION( "parallel vectors" ) {
        ac_ivec4 a = {1, 0, 0, 0};
        ac_ivec4 b = {1, 0, 0, 0};
        int result = ac_ivec4_dot(&a, &b);
        REQUIRE(result == 1);

        a = {2, 0, 0, 0};
        b = {4, 0, 0, 0};
        result = ac_ivec4_dot(&a, &b);
        REQUIRE(result == 8);
    }

    SECTION( "perpendicular vectors" ) {
        ac_ivec4 a = {1, 0, 0, 0};
        ac_ivec4 b = {0, 1, 0, 0};
        int result = ac_ivec4_dot(&a, &b);
        REQUIRE(result == 0);
    }

    SECTION( "opposite vectors" ) {
        ac_ivec4 a = {1, 0, 0, 0};
        ac_ivec4 b = {-1, 0, 0, 0};
        int result = ac_ivec4_dot(&a, &b);
        REQUIRE(result == -1);
    }

    SECTION( "non-orthogonal vectors" ) {
        ac_ivec4 a = {1, 0, 0, 0};
        ac_ivec4 b = {1, 1, 0, 0};
        int result = ac_ivec4_dot(&a, &b);
        REQUIRE(result == 1);
    }
}