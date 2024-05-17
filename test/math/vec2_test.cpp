#include <ace/math/math.h>
#include <ace/math/vec2.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <math.h>

//--------------------------------------------------------------------------------------------------
// float
//--------------------------------------------------------------------------------------------------

TEST_CASE( "ac_vec2 union aliases", "[ac_vec2]" ) {
    ac_vec2 v = {1.0f, 2.0f};
    REQUIRE(v.x == 1.0f);
    REQUIRE(v.y == 2.0f);
    REQUIRE(v.u == 1.0f);
    REQUIRE(v.v == 2.0f);
    REQUIRE(v.data[0] == 1.0f);
    REQUIRE(v.data[1] == 2.0f);
}

TEST_CASE( "ac_vec2_zero", "[ac_vec2]" ) {
    ac_vec2 zero = ac_vec2_zero();
    REQUIRE(zero.x == 0.0f);
    REQUIRE(zero.y == 0.0f);
}

TEST_CASE( "ac_vec2_nan", "[ac_vec2]" ) {
    ac_vec2 nan = ac_vec2_nan();
    REQUIRE(isnan(nan.x));
    REQUIRE(isnan(nan.y));
}

TEST_CASE( "ac_vec2_is_zero", "[ac_vec2]") {
    auto [input, expected] = GENERATE( Catch::Generators::table<ac_vec2, bool>({
        { ac_vec2_zero(), true },
        { {-0.0f, 0.0f }, true },
        { {0.0f, -0.0f }, true },

        { ac_vec2_nan(), false },
        { {1.0f, 0.0f }, false },
        { {0.0f, 1.0f }, false }
    }));

    CAPTURE(input.x, input.y);
    REQUIRE(ac_vec2_is_zero(&input) == expected);
}

TEST_CASE( "ac_vec2_is_nan", "[ac_vec2]" ) {
    SECTION( "nan vectors" ) {
        float x = GENERATE( Catch::Generators::values<float>({NAN, 0.0f}));
        float y = GENERATE( Catch::Generators::values<float>({NAN, 0.0f}));

        ac_vec2 v = {x, y};
        // we only want to check the generator expressions that are not all zero
        if (!ac_vec2_is_zero(&v))
        {
            CAPTURE( x, y );
            REQUIRE(ac_vec2_is_nan(&v) == true);
        }
    }

    SECTION( "non-nan vectors" ) {
        float x = GENERATE( Catch::Generators::values<float>({1.0f, 0.0f}));
        float y = GENERATE( Catch::Generators::values<float>({1.0f, 0.0f}));

        ac_vec2 v = {x, y};
        CAPTURE( x, y);
        REQUIRE(ac_vec2_is_nan(&v) == false);
    }
}

TEST_CASE( "ac_vec2_is_equal", "[ac_vec2]" ) {
    SECTION( "equal vectors" ) {
        float x = GENERATE( Catch::Generators::values<float>({1.0f, 0.0f}));
        float y = GENERATE( Catch::Generators::values<float>({1.0f, 0.0f}));

        ac_vec2 a = {x, y};
        ac_vec2 b = {x, y};
        CAPTURE( x, y);
        REQUIRE(ac_vec2_is_equal(&a, &b) == true);
    }

    SECTION( "non-equal vectors" ) {
        auto [a, b] = GENERATE( Catch::Generators::table<ac_vec2, ac_vec2>({
            { ac_vec2_nan(), ac_vec2_nan() },
            { ac_vec2_nan(), ac_vec2_zero() },
            { ac_vec2_zero(), ac_vec2_nan() },
            { ac_vec2_zero(), {1.0f, 0.0f} },
            { {1.0f, 0.0f}, ac_vec2_zero() },
            { {1.0f, 0.0f}, {0.0f, 1.0f} },
        }));

        CAPTURE(a.x, a.y, b.x, b.y);
        REQUIRE(ac_vec2_is_equal(&a, &b) == false);
    }
}

TEST_CASE( "ac_vec2_add", "[ac_vec2]" ) {
    ac_vec2 a = {1.0f, 2.0f};
    ac_vec2 b = {4.0f, 5.0f};
    ac_vec2 ab_result = {5.0f, 7.0f};
    ac_vec2 result = ac_vec2_add(&a, &b);
    REQUIRE(ac_vec2_is_equal(&result, &ab_result) == true);
}

TEST_CASE( "ac_vec2_sub", "[ac_vec2]" ) {
    ac_vec2 a = {1.0f, 2.0f};
    ac_vec2 b = {4.0f, 5.0f};
    ac_vec2 ab_result = {-3.0f, -3.0f};
    ac_vec2 result = ac_vec2_sub(&a, &b);
    REQUIRE(ac_vec2_is_equal(&result, &ab_result) == true);
}

TEST_CASE( "ac_vec2_negate", "[ac_vec2]" ) {
    ac_vec2 a = {1.0f, 2.0f};
    ac_vec2 a_result = {-1.0f, -2.0f};
    ac_vec2 result = ac_vec2_negate(&a);
    REQUIRE(ac_vec2_is_equal(&result, &a_result) == true);
}

TEST_CASE( "ac_vec2_scale", "[ac_vec2]" ) {
    ac_vec2 v = {1.0f, 2.0f};

    SECTION( "scaling by 0" ) {
        ac_vec2 expected = {0.0f, 0.0f};
        ac_vec2 result = ac_vec2_scale(&v, 0.0f);
        REQUIRE(ac_vec2_is_equal(&result, &expected) == true);
    }

    SECTION( "scaling by 1" ) {
        ac_vec2 expected = {1.0f, 2.0f};
        ac_vec2 result = ac_vec2_scale(&v, 1.0f);
        REQUIRE(ac_vec2_is_equal(&result, &expected) == true);
    }

    SECTION( "scaling by 2" ) {
        ac_vec2 expected = {2.0f, 4.0f};
        ac_vec2 result = ac_vec2_scale(&v, 2.0f);
        REQUIRE(ac_vec2_is_equal(&result, &expected) == true);
    }

    SECTION( "scaling by -1" ) {
        ac_vec2 expected = {-1.0f, -2.0f};
        ac_vec2 result = ac_vec2_scale(&v, -1.0f);
        REQUIRE(ac_vec2_is_equal(&result, &expected) == true);
    }

    SECTION( "scaling zero vector" ) {
        ac_vec2 zv = {0.0f, 0.0f};
        ac_vec2 expected = {0.0f, 0.0f};
        ac_vec2 result = ac_vec2_scale(&zv, 2.0f);
        REQUIRE(ac_vec2_is_equal(&result, &expected) == true);
    }
}

TEST_CASE( "ac_vec2_dot", "[ac_vec2]" ) {
    SECTION( "parallel vectors" ) {
        ac_vec2 a = {1.0f, 0.0f};
        ac_vec2 b = {1.0f, 0.0f};
        float result = ac_vec2_dot(&a, &b);
        REQUIRE(result == 1.0f);

        a = {2.0f, 0.0f};
        b = {4.0f, 0.0f};
        result = ac_vec2_dot(&a, &b);
        REQUIRE(result == 8.0f);
    }

    SECTION( "perpendicular vectors" ) {
        ac_vec2 a = {1.0f, 0.0f};
        ac_vec2 b = {0.0f, 1.0f};
        float result = ac_vec2_dot(&a, &b);
        REQUIRE(result == 0.0f);
    }

    SECTION( "opposite vectors" ) {
        ac_vec2 a = {1.0f, 0.0f};
        ac_vec2 b = {-1.0f, 0.0f};
        float result = ac_vec2_dot(&a, &b);
        REQUIRE(result == -1.0f);
    }

    SECTION( "non-orthogonal vectors" ) {
        ac_vec2 a = {1.0f, 0.0f};
        ac_vec2 b = {1.0f, 1.0f};
        float result = ac_vec2_dot(&a, &b);
        REQUIRE(result == 1.0f);
    }
}

TEST_CASE( "ac_vec2_magnitude", "[ac_vec2]" ) {
    SECTION( "zero vector" ) {
        ac_vec2 zero = ac_vec2_zero();
        float result = ac_vec2_magnitude(&zero);
        REQUIRE(result == 0.0f);
    }

    SECTION( "unit vector" ) {
        ac_vec2 unit = {1.0f, 0.0f};
        float result = ac_vec2_magnitude(&unit);
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(1.0f, AC_EPSILON) ||
            Catch::Matchers::WithinAbs(1.0f, AC_EPSILON)
        );
    }

    SECTION( "non-unit vector" ) {
        ac_vec2 v = {1.0f, 1.0f};
        float result = ac_vec2_magnitude(&v);
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(1.41421356f, AC_EPSILON) ||
            Catch::Matchers::WithinAbs(1.41421356f, AC_EPSILON)
        );
    }
}

TEST_CASE( "ac_vec2_normalize", "[ac_vec2]" ) {
    SECTION( "nan vector" ) {
        ac_vec2 nan = ac_vec2_nan();
        ac_vec2 result = ac_vec2_normalize(&nan);
        REQUIRE(ac_vec2_is_nan(&result) == true);
    }

    SECTION( "zero vector" ) {
        ac_vec2 zero = ac_vec2_zero();
        ac_vec2 result = ac_vec2_normalize(&zero);
        REQUIRE(ac_vec2_is_nan(&result) == true);
    }

    SECTION( "unit vector" ) {
        ac_vec2 unit = {1.0f, 0.0f};
        ac_vec2 expected = {1.0f, 0.0f};
        ac_vec2 result = ac_vec2_normalize(&unit);
        REQUIRE(ac_vec2_is_equal(&result, &expected) == true);
    }

    SECTION( "non-unit vector" ) {
        ac_vec2 v = {1.0f, 1.0f};
        float result = ac_vec2_magnitude(&v);
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(1.41421356f, AC_EPSILON) ||
            Catch::Matchers::WithinAbs(1.41421356f, AC_EPSILON)
        );
    }
}

//--------------------------------------------------------------------------------------------------
// int
//--------------------------------------------------------------------------------------------------

TEST_CASE( "ac_ivec2 union aliases", "[ac_vec2]" ) {
    ac_ivec2 v = {1, 2};
    REQUIRE(v.x == 1);
    REQUIRE(v.y == 2);
    REQUIRE(v.u == 1);
    REQUIRE(v.v == 2);
    REQUIRE(v.data[0] == 1);
    REQUIRE(v.data[1] == 2);
}

TEST_CASE( "ac_ivec2_zero", "[ac_vec2]" ) {
    ac_ivec2 zero = ac_ivec2_zero();
    REQUIRE(zero.x == 0);
    REQUIRE(zero.y == 0);
}

TEST_CASE( "ac_ivec2_invalid", "[ac_vec2]" ) {
    ac_ivec2 invalid = ac_ivec2_invalid();
    REQUIRE(invalid.x == INT_INVALID);
    REQUIRE(invalid.y == INT_INVALID);
}

TEST_CASE( "ac_ivec2_is_zero", "[ac_vec2]" ) {
    auto [input, expected] = GENERATE( Catch::Generators::table<ac_ivec2, bool>({
        { ac_ivec2_zero(), true },
        { {-0, 0}, true },
        { {0, -0}, true },

        { ac_ivec2_invalid(), false },
        { {1, 0}, false },
        { {0, 1}, false },
    }));

    CAPTURE(input.x, input.y);
    REQUIRE(ac_ivec2_is_zero(&input) == expected);
}

TEST_CASE( "ac_ivec2_is_invalid", "[ac_vec2]" ) {
    SECTION( "invalid vectors" ) {
        int x = GENERATE( Catch::Generators::values<int>({INT_INVALID, 0}));
        int y = GENERATE( Catch::Generators::values<int>({INT_INVALID, 0}));

        ac_ivec2 v = {x, y};
        // we only want to check the generator expressions that are not all zero
        if (!ac_ivec2_is_zero(&v))
        {
            CAPTURE( x, y );
            REQUIRE(ac_ivec2_is_invalid(&v) == true);
        }
    }

    SECTION( "valid vectors" ) {
        int x = GENERATE( Catch::Generators::values<int>({1, 0}));
        int y = GENERATE( Catch::Generators::values<int>({1, 0}));

        ac_ivec2 v = {x, y};
        CAPTURE( x, y );
        REQUIRE(ac_ivec2_is_invalid(&v) == false);
    }
}

TEST_CASE( "ac_ivec2_is_equal", "[ac_vec2]" ) {
    SECTION( "equal vectors" ) {
        int x = GENERATE( Catch::Generators::values<int>({1, 0}));
        int y = GENERATE( Catch::Generators::values<int>({1, 0}));

        ac_ivec2 a = {x, y};
        ac_ivec2 b = {x, y};
        CAPTURE( x, y );
        REQUIRE(ac_ivec2_is_equal(&a, &b) == true);
    }

    SECTION( "non-equal vectors" ) {
        auto [a, b] = GENERATE( Catch::Generators::table<ac_ivec2, ac_ivec2>({
            { ac_ivec2_invalid(), ac_ivec2_invalid() },
            { ac_ivec2_invalid(), ac_ivec2_zero() },
            { ac_ivec2_zero(), ac_ivec2_invalid() },
            { ac_ivec2_zero(), {1, 0} },
            { {1, 0}, ac_ivec2_zero() },
            { {1, 0}, {0, 1} },
        }));

        CAPTURE(a.x, a.y, b.x, b.y);
        REQUIRE(ac_ivec2_is_equal(&a, &b) == false);
    }
}

TEST_CASE( "ac_ivec2_add", "[ac_vec2]" ) {
    SECTION( "invalid vectors" ) {
        ac_ivec2 invalid = ac_ivec2_invalid();
        ac_ivec2 v = { 1, 2 };

        ac_ivec2 result = ac_ivec2_add(&invalid, &v);
        REQUIRE(ac_ivec2_is_invalid(&result) == true);

        result = ac_ivec2_add(&v, &invalid);
        REQUIRE(ac_ivec2_is_invalid(&result) == true);
    }

    SECTION( "valid vectors" ) {
        ac_ivec2 a = {1, 2};
        ac_ivec2 b = {4, 5};
        ac_ivec2 ab_result = {5, 7};
        ac_ivec2 result = ac_ivec2_add(&a, &b);
        REQUIRE(ac_ivec2_is_equal(&result, &ab_result) == true);
    }
}

TEST_CASE( "ac_ivec2_sub", "[ac_vec2]" ) {
    SECTION( "invalid vectors" ) {
        ac_ivec2 invalid = ac_ivec2_invalid();
        ac_ivec2 v = { 1, 2 };

        ac_ivec2 result = ac_ivec2_sub(&invalid, &v);
        REQUIRE(ac_ivec2_is_invalid(&result) == true);

        result = ac_ivec2_sub(&v, &invalid);
        REQUIRE(ac_ivec2_is_invalid(&result) == true);
    }

    SECTION( "valid vectors" ) {
        ac_ivec2 a = {1, 2};
        ac_ivec2 b = {4, 5};
        ac_ivec2 ab_result = {-3, -3};
        ac_ivec2 result = ac_ivec2_sub(&a, &b);
        REQUIRE(ac_ivec2_is_equal(&result, &ab_result) == true);
    }
}

TEST_CASE( "ac_ivec2_negate", "[ac_vec2]" ) {
    SECTION( "invalid vector" ) {
        ac_ivec2 invalid = ac_ivec2_invalid();
        ac_ivec2 result = ac_ivec2_negate(&invalid);
        REQUIRE(ac_ivec2_is_invalid(&result) == true);
    }

    SECTION( "valid vector" ) {
        ac_ivec2 a = {1, 2};
        ac_ivec2 a_result = {-1, -2};
        ac_ivec2 result = ac_ivec2_negate(&a);
        REQUIRE(ac_ivec2_is_equal(&result, &a_result) == true);
    }
}

TEST_CASE( "ac_ivec2_scale", "[ac_vec2]" ) {
    SECTION( "invalid vector" ) {
        ac_ivec2 invalid = ac_ivec2_invalid();
        ac_ivec2 result = ac_ivec2_scale(&invalid, 2);
        REQUIRE(ac_ivec2_is_invalid(&result) == true);
    }

    SECTION( "zero vector" ) {
        ac_ivec2 zero = ac_ivec2_zero();
        ac_ivec2 result = ac_ivec2_scale(&zero, 2);
        REQUIRE(ac_ivec2_is_zero(&result) == true);
    }

    SECTION( "non-zero vector" ) {
        ac_ivec2 v = {1, 2};
        ac_ivec2 expected = {2, 4};
        ac_ivec2 result = ac_ivec2_scale(&v, 2);
        REQUIRE(ac_ivec2_is_equal(&result, &expected) == true);
    }
}

TEST_CASE( "ac_ivec2_divide", "[ac_vec2]" ) {
    SECTION( "invalid vector" ) {
        ac_ivec2 invalid = ac_ivec2_invalid();
        ac_ivec2 result = ac_ivec2_divide(&invalid, 2);
        REQUIRE(ac_ivec2_is_invalid(&result) == true);
    }

    SECTION( "zero vector" ) {
        ac_ivec2 zero = ac_ivec2_zero();
        ac_ivec2 result = ac_ivec2_divide(&zero, 2);
        REQUIRE(ac_ivec2_is_zero(&result) == true);
    }

    SECTION( "non-zero vector" ) {
        ac_ivec2 v = {2, 4};
        ac_ivec2 expected = {1, 2};
        ac_ivec2 result = ac_ivec2_divide(&v, 2);
        REQUIRE(ac_ivec2_is_equal(&result, &expected) == true);
    }

    SECTION( "zero scalar" ) {
        ac_ivec2 v = {2, 4};
        ac_ivec2 result = ac_ivec2_divide(&v, 0);
        REQUIRE(ac_ivec2_is_invalid(&result) == true);
    }

    SECTION( "negative scalar" ) {
        ac_ivec2 v = {2, 4};
        ac_ivec2 expected = {-2, -4};
        ac_ivec2 result = ac_ivec2_divide(&v, -1);
        REQUIRE(ac_ivec2_is_equal(&result, &expected) == true);
    }

    SECTION( "integer division" ) {
        ac_ivec2 v = {5, 10};
        ac_ivec2 expected = {2, 5};
        ac_ivec2 result = ac_ivec2_divide(&v, 2);
        REQUIRE(ac_ivec2_is_equal(&result, &expected) == true);
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

TEST_CASE( "ac_ivec2_divide_ext", "[ac_vec2]" ) {
    SECTION( "invalid vector" ) {
        ac_ivec2 invalid = ac_ivec2_invalid();
        ac_ivec2 result = ac_ivec2_divide_ext(&invalid, 2, round_func_nearest);
        REQUIRE(ac_ivec2_is_invalid(&result) == true);
    }

    SECTION( "zero vector" ) {
        ac_ivec2 zero = ac_ivec2_zero();
        ac_ivec2 result = ac_ivec2_divide_ext(&zero, 2, round_func_nearest);
        REQUIRE(ac_ivec2_is_zero(&result) == true);
    }

    SECTION( "zero scalar" ) {
        ac_ivec2 v = {2, 4};
        ac_ivec2 result = ac_ivec2_divide_ext(&v, 0, round_func_nearest);
        REQUIRE(ac_ivec2_is_invalid(&result) == true);
    }

    SECTION( "round up" ) {
        ac_ivec2 v = {5, 10};
        ac_ivec2 expected = {2, 4};
        ac_ivec2 result = ac_ivec2_divide_ext(&v, 3, round_func_up);
        REQUIRE(ac_ivec2_is_equal(&result, &expected) == true);
    }

    SECTION( "round down" ) {
        ac_ivec2 v = {5, 10};
        ac_ivec2 expected = {1, 3};
        ac_ivec2 result = ac_ivec2_divide_ext(&v, 3, round_func_down);
        REQUIRE(ac_ivec2_is_equal(&result, &expected) == true);
    }

    SECTION( "round nearest" ) {
        ac_ivec2 v = {5, 10};
        ac_ivec2 expected = {2, 3};
        ac_ivec2 result = ac_ivec2_divide_ext(&v, 3, round_func_nearest);
        REQUIRE(ac_ivec2_is_equal(&result, &expected) == true);
    }
}

TEST_CASE( "ac_ivec2_dot", "[ac_vec2]" ) {
    SECTION( "invalid vector" ) {
        ac_ivec2 invalid = ac_ivec2_invalid();
        ac_ivec2 v = {1, 0};

        int result = ac_ivec2_dot(&invalid, &v);
        REQUIRE( result == INT_INVALID);

        result = ac_ivec2_dot(&v, &invalid);
        REQUIRE( result == INT_INVALID);
    }

    SECTION( "parallel vectors" ) {
        ac_ivec2 a = {1, 0};
        ac_ivec2 b = {1, 0};
        int result = ac_ivec2_dot(&a, &b);
        REQUIRE(result == 1);

        a = {2, 0};
        b = {4, 0};
        result = ac_ivec2_dot(&a, &b);
        REQUIRE(result == 8);
    }

    SECTION( "perpendicular vectors" ) {
        ac_ivec2 a = {1, 0};
        ac_ivec2 b = {0, 1};
        int result = ac_ivec2_dot(&a, &b);
        REQUIRE(result == 0);
    }

    SECTION( "opposite vectors" ) {
        ac_ivec2 a = {1, 0};
        ac_ivec2 b = {-1, 0};
        int result = ac_ivec2_dot(&a, &b);
        REQUIRE(result == -1);
    }

    SECTION( "non-orthogonal vectors" ) {
        ac_ivec2 a = {1, 0};
        ac_ivec2 b = {1, 1};
        int result = ac_ivec2_dot(&a, &b);
        REQUIRE(result == 1);
    }
}
