#include <alibrary/math/math.h>
#include <alibrary/math/vec3.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <math.h>

//--------------------------------------------------------------------------------------------------
// float
//--------------------------------------------------------------------------------------------------

TEST_CASE( "vec3 union aliases", "[vec3]" ) {
    vec3 v = {1.0f, 2.0f, 3.0f};
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

TEST_CASE( "vec3_zero", "[vec3]" ) {
    vec3 zero = vec3_zero();
    REQUIRE(zero.x == 0.0f);
    REQUIRE(zero.y == 0.0f);
    REQUIRE(zero.z == 0.0f);
}

TEST_CASE( "vec3_nan", "[vec3]" ) {
    vec3 nan = vec3_nan();
    REQUIRE(isnan(nan.x));
    REQUIRE(isnan(nan.y));
    REQUIRE(isnan(nan.z));
}

TEST_CASE( "vec3_is_zero", "[vec3]") {
    auto [input, expected] = GENERATE( Catch::Generators::table<vec3, bool>({
        { vec3_zero(), true },
        { {-0.0f, 0.0f, 0.0f }, true },
        { {0.0f, -0.0f, 0.0f }, true },
        { {0.0f, 0.0f, -0.0f }, true },

        { vec3_nan(), false },
        { {1.0f, 0.0f, 0.0f }, false },
        { {0.0f, 1.0f, 0.0f }, false },
        { {0.0f, 0.0f, 1.0f }, false },
    }));

    CAPTURE(input.x, input.y, input.z);
    REQUIRE(vec3_is_zero(&input) == expected);
}

TEST_CASE( "vec3_is_nan", "[vec3]" ) {
    SECTION( "nan vectors" ) {
        float x = GENERATE( Catch::Generators::values<float>({NAN, 0.0f}));
        float y = GENERATE( Catch::Generators::values<float>({NAN, 0.0f}));
        float z = GENERATE( Catch::Generators::values<float>({NAN, 0.0f}));

        vec3 v = {x, y, z};
        // we only want to check the generator expressions that are not all zero
        if (!vec3_is_zero(&v))
        {
            CAPTURE( x, y, z );
            REQUIRE(vec3_is_nan(&v) == true);
        }
    }

    SECTION( "non-nan vectors" ) {
        float x = GENERATE( Catch::Generators::values<float>({1.0f, 0.0f}));
        float y = GENERATE( Catch::Generators::values<float>({1.0f, 0.0f}));
        float z = GENERATE( Catch::Generators::values<float>({1.0f, 0.0f}));

        vec3 v = {x, y, z};
        CAPTURE( x, y, z );
        REQUIRE(vec3_is_nan(&v) == false);
    }
}

TEST_CASE( "vec3_is_equal", "[vec3]" ) {
    SECTION( "equal vectors" ) {
        float x = GENERATE( Catch::Generators::values<float>({1.0f, 0.0f}));
        float y = GENERATE( Catch::Generators::values<float>({1.0f, 0.0f}));
        float z = GENERATE( Catch::Generators::values<float>({1.0f, 0.0f}));

        vec3 a = {x, y, z};
        vec3 b = {x, y, z};
        CAPTURE( x, y, z );
        REQUIRE(vec3_is_equal(&a, &b) == true);
    }

    SECTION( "non-equal vectors" ) {
        auto [a, b] = GENERATE( Catch::Generators::table<vec3, vec3>({
            { vec3_nan(), vec3_nan() },
            { vec3_nan(), vec3_zero() },
            { vec3_zero(), vec3_nan() },
            { vec3_zero(), {1.0f, 0.0f, 0.0f} },
            { {1.0f, 0.0f, 0.0f}, vec3_zero() },
            { {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} },
            { {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f} }
        }));

        CAPTURE(a.x, a.y, a.z, b.x, b.y, b.z);
        REQUIRE(vec3_is_equal(&a, &b) == false);
    }
}

TEST_CASE( "vec3_add", "[vec3]" ) {
    vec3 a = {1.0f, 2.0f, 3.0f};
    vec3 b = {4.0f, 5.0f, 6.0f};
    vec3 ab_result = {5.0f, 7.0f, 9.0f};
    vec3 result = vec3_add(&a, &b);
    REQUIRE(vec3_is_equal(&result, &ab_result) == true);
}

TEST_CASE( "vec3_sub", "[vec3]" ) {
    vec3 a = {1.0f, 2.0f, 3.0f};
    vec3 b = {4.0f, 5.0f, 6.0f};
    vec3 ab_result = {-3.0f, -3.0f, -3.0f};
    vec3 result = vec3_sub(&a, &b);
    REQUIRE(vec3_is_equal(&result, &ab_result) == true);
}

TEST_CASE( "vec3_negate", "[vec3]" ) {
    vec3 a = {1.0f, 2.0f, 3.0f};
    vec3 a_result = {-1.0f, -2.0f, -3.0f};
    vec3 result = vec3_negate(&a);
    REQUIRE(vec3_is_equal(&result, &a_result) == true);
}

TEST_CASE( "vec3_scale", "[vec3]" ) {
    vec3 v = {1.0f, 2.0f, 3.0f};

    SECTION( "scaling by 0" ) {
        vec3 expected = {0.0f, 0.0f, 0.0f};
        vec3 result = vec3_scale(&v, 0.0f);
        REQUIRE(vec3_is_equal(&result, &expected) == true);
    }

    SECTION( "scaling by 1" ) {
        vec3 expected = {1.0f, 2.0f, 3.0f};
        vec3 result = vec3_scale(&v, 1.0f);
        REQUIRE(vec3_is_equal(&result, &expected) == true);
    }

    SECTION( "scaling by 2" ) {
        vec3 expected = {2.0f, 4.0f, 6.0f};
        vec3 result = vec3_scale(&v, 2.0f);
        REQUIRE(vec3_is_equal(&result, &expected) == true);
    }

    SECTION( "scaling by -1" ) {
        vec3 expected = {-1.0f, -2.0f, -3.0f};
        vec3 result = vec3_scale(&v, -1.0f);
        REQUIRE(vec3_is_equal(&result, &expected) == true);
    }

    SECTION( "scaling zero vector" ) {
        vec3 zv = {0.0f, 0.0f, 0.0f};
        vec3 expected = {0.0f, 0.0f, 0.0f};
        vec3 result = vec3_scale(&zv, 2.0f);
        REQUIRE(vec3_is_equal(&result, &expected) == true);
    }
}

TEST_CASE( "vec3_dot", "[vec3]" ) {
    SECTION( "parallel vectors" ) {
        vec3 a = {1.0f, 0.0f, 0.0f};
        vec3 b = {1.0f, 0.0f, 0.0f};
        float result = vec3_dot(&a, &b);
        REQUIRE(result == 1.0f);

        a = {2.0f, 0.0f, 0.0f};
        b = {4.0f, 0.0f, 0.0f};
        result = vec3_dot(&a, &b);
        REQUIRE(result == 8.0f);
    }

    SECTION( "perpendicular vectors" ) {
        vec3 a = {1.0f, 0.0f, 0.0f};
        vec3 b = {0.0f, 1.0f, 0.0f};
        float result = vec3_dot(&a, &b);
        REQUIRE(result == 0.0f);
    }

    SECTION( "opposite vectors" ) {
        vec3 a = {1.0f, 0.0f, 0.0f};
        vec3 b = {-1.0f, 0.0f, 0.0f};
        float result = vec3_dot(&a, &b);
        REQUIRE(result == -1.0f);
    }

    SECTION( "non-orthogonal vectors" ) {
        vec3 a = {1.0f, 0.0f, 0.0f};
        vec3 b = {1.0f, 1.0f, 0.0f};
        float result = vec3_dot(&a, &b);
        REQUIRE(result == 1.0f);
    }
}

TEST_CASE( "vec3_cross", "[vec3]" ) {
    SECTION( "parallel vectors" ) {
        vec3 a = {1.0f, 0.0f, 0.0f};
        vec3 b = {1.0f, 0.0f, 0.0f};
        vec3 result = vec3_cross(&a, &b);
        REQUIRE(vec3_is_zero(&result) == true);
    }

    SECTION( "perpendicular vectors" ) {
        vec3 a = {1.0f, 0.0f, 0.0f};
        vec3 b = {0.0f, 1.0f, 0.0f};
        vec3 result = vec3_cross(&a, &b);
        REQUIRE(result.x == 0.0f);
        REQUIRE(result.y == 0.0f);
        REQUIRE(result.z == 1.0f);
    }

    SECTION( "non-orthogonal vectors" ) {
        vec3 a = {1.0f, 0.0f, 0.0f};
        vec3 b = {0.0f, 1.0f, 0.0f};
        vec3 result = vec3_cross(&a, &b);
        REQUIRE(result.x == 0.0f);
        REQUIRE(result.y == 0.0f);
        REQUIRE(result.z == 1.0f);
    }
}

TEST_CASE( "vec3_magnitude", "[vec3]" ) {
    SECTION( "zero vector" ) {
        vec3 zero = vec3_zero();
        float result = vec3_magnitude(&zero);
        REQUIRE(result == 0.0f);
    }

    SECTION( "unit vector" ) {
        vec3 unit = {1.0f, 0.0f, 0.0f};
        float result = vec3_magnitude(&unit);
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(1.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(1.0f, AL_EPSILON)
        );
    }

    SECTION( "non-unit vector" ) {
        vec3 v = {1.0f, 1.0f, 1.0f};
        float result = vec3_magnitude(&v);
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(1.732051f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(1.732051f, AL_EPSILON)
        );
    }
}

TEST_CASE( "vec3_normalize", "[vec3]" ) {
    SECTION( "nan vector" ) {
        vec3 nan = vec3_nan();
        vec3 result = vec3_normalize(&nan);
        REQUIRE(vec3_is_nan(&result) == true);
    }

    SECTION( "zero vector" ) {
        vec3 zero = vec3_zero();
        vec3 result = vec3_normalize(&zero);
        REQUIRE(vec3_is_nan(&result) == true);
    }

    SECTION( "unit vector" ) {
        vec3 unit = {1.0f, 0.0f, 0.0f};
        vec3 expected = {1.0f, 0.0f, 0.0f};
        vec3 result = vec3_normalize(&unit);
        REQUIRE(vec3_is_equal(&result, &expected) == true);
    }

    SECTION( "non-unit vector" ) {
        vec3 v = {1.0f, 1.0f, 1.0f};
        vec3 expected = {0.577350f, 0.577350f, 0.577350f};
        vec3 result = vec3_normalize(&v);
        REQUIRE( vec3_is_equal(&result, &expected) == true );
    }
}

//--------------------------------------------------------------------------------------------------
// int
//--------------------------------------------------------------------------------------------------

TEST_CASE( "ivec3 union aliases", "[vec3]" ) {
    ivec3 v = {1, 2, 3};
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

TEST_CASE( "ivec3_zero", "[vec3]" ) {
    ivec3 zero = ivec3_zero();
    REQUIRE(zero.x == 0);
    REQUIRE(zero.y == 0);
    REQUIRE(zero.z == 0);
}

TEST_CASE( "ivec3_nan", "[vec3]" ) {
    ivec3 nan = ivec3_nan();
    REQUIRE(nan.x == INT_NAN);
    REQUIRE(nan.y == INT_NAN);
    REQUIRE(nan.z == INT_NAN);
}

TEST_CASE( "ivec3_is_zero", "[vec3]" ) {
    auto [input, expected] = GENERATE( Catch::Generators::table<ivec3, bool>({
        { ivec3_zero(), true },
        { {-0, 0, 0}, true },
        { {0, -0, 0}, true },
        { {0, 0, -0}, true },

        { ivec3_nan(), false },
        { {1, 0, 0}, false },
        { {0, 1, 0}, false },
        { {0, 0, 1}, false },
    }));

    CAPTURE(input.x, input.y, input.z);
    REQUIRE(ivec3_is_zero(&input) == expected);
}

TEST_CASE( "ivec3_is_nan", "[vec3]" ) {
    SECTION( "nan vectors" ) {
        int x = GENERATE( Catch::Generators::values<int>({INT_NAN, 0}));
        int y = GENERATE( Catch::Generators::values<int>({INT_NAN, 0}));
        int z = GENERATE( Catch::Generators::values<int>({INT_NAN, 0}));

        ivec3 v = {x, y, z};
        // we only want to check the generator expressions that are not all zero
        if (!ivec3_is_zero(&v))
        {
            CAPTURE( x, y, z );
            REQUIRE(ivec3_is_nan(&v) == true);
        }
    }

    SECTION( "non-nan vectors" ) {
        int x = GENERATE( Catch::Generators::values<int>({1, 0}));
        int y = GENERATE( Catch::Generators::values<int>({1, 0}));
        int z = GENERATE( Catch::Generators::values<int>({1, 0}));

        ivec3 v = {x, y, z};
        CAPTURE( x, y, z );
        REQUIRE(ivec3_is_nan(&v) == false);
    }
}

TEST_CASE( "ivec3_is_equal", "[vec3]" ) {
    SECTION( "equal vectors" ) {
        int x = GENERATE( Catch::Generators::values<int>({1, 0}));
        int y = GENERATE( Catch::Generators::values<int>({1, 0}));
        int z = GENERATE( Catch::Generators::values<int>({1, 0}));

        ivec3 a = {x, y, z};
        ivec3 b = {x, y, z};
        CAPTURE( x, y, z );
        REQUIRE(ivec3_is_equal(&a, &b) == true);
    }

    SECTION( "non-equal vectors" ) {
        auto [a, b] = GENERATE( Catch::Generators::table<ivec3, ivec3>({
            { ivec3_nan(), ivec3_nan() },
            { ivec3_nan(), ivec3_zero() },
            { ivec3_zero(), ivec3_nan() },
            { ivec3_zero(), {1, 0, 0} },
            { {1, 0, 0}, ivec3_zero() },
            { {1, 0, 0}, {0, 1, 0} },
            { {1, 0, 0}, {0, 0, 1} }
        }));

        CAPTURE(a.x, a.y, a.z, b.x, b.y, b.z);
        REQUIRE(ivec3_is_equal(&a, &b) == false);
    }
}

TEST_CASE( "ivec3_add", "[vec3]" ) {
    SECTION( "nan vectors" ) {
        ivec3 nan = ivec3_nan();
        ivec3 v = { 1, 2, 3 };

        ivec3 result = ivec3_add(&nan, &v);
        REQUIRE(ivec3_is_nan(&result) == true);

        result = ivec3_add(&v, &nan);
        REQUIRE(ivec3_is_nan(&result) == true);
    }

    SECTION( "non-nan vectors" ) {
        ivec3 a = {1, 2, 3};
        ivec3 b = {4, 5, 6};
        ivec3 ab_result = {5, 7, 9};
        ivec3 result = ivec3_add(&a, &b);
        REQUIRE(ivec3_is_equal(&result, &ab_result) == true);
    }
}

TEST_CASE( "ivec3_sub", "[vec3]" ) {
    SECTION( "nan vectors" ) {
        ivec3 nan = ivec3_nan();
        ivec3 v = { 1, 2, 3 };

        ivec3 result = ivec3_sub(&nan, &v);
        REQUIRE(ivec3_is_nan(&result) == true);

        result = ivec3_sub(&v, &nan);
        REQUIRE(ivec3_is_nan(&result) == true);
    }

    SECTION( "non-nan vectors" ) {
        ivec3 a = {1, 2, 3};
        ivec3 b = {4, 5, 6};
        ivec3 ab_result = {-3, -3, -3};
        ivec3 result = ivec3_sub(&a, &b);
        REQUIRE(ivec3_is_equal(&result, &ab_result) == true);
    }
}

TEST_CASE( "ivec3_negate", "[vec3]" ) {
    SECTION( "nan vector" ) {
        ivec3 nan = ivec3_nan();
        ivec3 result = ivec3_negate(&nan);
        REQUIRE(ivec3_is_nan(&result) == true);
    }

    SECTION( "non-nan vector" ) {
        ivec3 a = {1, 2, 3};
        ivec3 a_result = {-1, -2, -3};
        ivec3 result = ivec3_negate(&a);
        REQUIRE(ivec3_is_equal(&result, &a_result) == true);
    }
}

TEST_CASE( "ivec3_scale", "[vec3]" ) {
    SECTION( "nan vector" ) {
        ivec3 nan = ivec3_nan();
        ivec3 result = ivec3_scale(&nan, 2);
        REQUIRE(ivec3_is_nan(&result) == true);
    }

    SECTION( "zero vector" ) {
        ivec3 zero = ivec3_zero();
        ivec3 result = ivec3_scale(&zero, 2);
        REQUIRE(ivec3_is_zero(&result) == true);
    }

    SECTION( "non-zero vector" ) {
        ivec3 v = {1, 2, 3};
        ivec3 expected = {2, 4, 6};
        ivec3 result = ivec3_scale(&v, 2);
        REQUIRE(ivec3_is_equal(&result, &expected) == true);
    }
}

TEST_CASE( "ivec3_divide", "[vec3]" ) {
    SECTION( "nan vector" ) {
        ivec3 nan = ivec3_nan();
        ivec3 result = ivec3_divide(&nan, 2);
        REQUIRE(ivec3_is_nan(&result) == true);
    }

    SECTION( "zero vector" ) {
        ivec3 zero = ivec3_zero();
        ivec3 result = ivec3_divide(&zero, 2);
        REQUIRE(ivec3_is_zero(&result) == true);
    }

    SECTION( "non-zero vector" ) {
        ivec3 v = {2, 4, 6};
        ivec3 expected = {1, 2, 3};
        ivec3 result = ivec3_divide(&v, 2);
        REQUIRE(ivec3_is_equal(&result, &expected) == true);
    }

    SECTION( "zero scalar" ) {
        ivec3 v = {2, 4, 6};
        ivec3 result = ivec3_divide(&v, 0);
        REQUIRE(ivec3_is_nan(&result) == true);
    }

    SECTION( "negative scalar" ) {
        ivec3 v = {2, 4, 6};
        ivec3 expected = {-2, -4, -6};
        ivec3 result = ivec3_divide(&v, -1);
        REQUIRE(ivec3_is_equal(&result, &expected) == true);
    }

    SECTION( "integer division" ) {
        ivec3 v = {5, 10, 15};
        ivec3 expected = {2, 5, 7};
        ivec3 result = ivec3_divide(&v, 2);
        REQUIRE(ivec3_is_equal(&result, &expected) == true);
    }
}

int round_func_up(float value)
{
    return (int)ceilf(value);
}

int round_func_down(float value)
{
    return (int)floorf(value);
}

int round_func_nearest(float value)
{
    return (int)roundf(value);
}

TEST_CASE( "ivec3_divide_ext", "[vec3]" ) {
    SECTION( "nan vector" ) {
        ivec3 nan = ivec3_nan();
        ivec3 result = ivec3_divide_ext(&nan, 2, round_func_nearest);
        REQUIRE(ivec3_is_nan(&result) == true);
    }

    SECTION( "zero vector" ) {
        ivec3 zero = ivec3_zero();
        ivec3 result = ivec3_divide_ext(&zero, 2, round_func_nearest);
        REQUIRE(ivec3_is_zero(&result) == true);
    }

    SECTION( "zero scalar" ) {
        ivec3 v = {2, 4, 6};
        ivec3 result = ivec3_divide_ext(&v, 0, round_func_nearest);
        REQUIRE(ivec3_is_nan(&result) == true);
    }

    SECTION( "round up" ) {
        ivec3 v = {5, 10, 14};
        ivec3 expected = {2, 4, 5};
        ivec3 result = ivec3_divide_ext(&v, 3, round_func_up);
    }

    SECTION( "round down" ) {
        ivec3 v = {5, 10, 14};
        ivec3 expected = {1, 3, 4};
        ivec3 result = ivec3_divide_ext(&v, 3, round_func_down);
    }

    SECTION( "round nearest" ) {
        ivec3 v = {5, 10, 14};
        ivec3 expected = {2, 3, 5};
        ivec3 result = ivec3_divide_ext(&v, 3, round_func_nearest);
    }
}

TEST_CASE( "ivec3_dot", "[vec3]" ) {
    SECTION( "nan vector" ) {
        ivec3 nan = ivec3_nan();
        ivec3 v = {1, 0, 0};

        int result = ivec3_dot(&nan, &v);
        REQUIRE( result == INT_NAN);

        result = ivec3_dot(&v, &nan);
        REQUIRE( result == INT_NAN);
    }

    SECTION( "parallel vectors" ) {
        ivec3 a = {1, 0, 0};
        ivec3 b = {1, 0, 0};
        int result = ivec3_dot(&a, &b);
        REQUIRE(result == 1);

        a = {2, 0, 0};
        b = {4, 0, 0};
        result = ivec3_dot(&a, &b);
        REQUIRE(result == 8);
    }

    SECTION( "perpendicular vectors" ) {
        ivec3 a = {1, 0, 0};
        ivec3 b = {0, 1, 0};
        int result = ivec3_dot(&a, &b);
        REQUIRE(result == 0);
    }

    SECTION( "opposite vectors" ) {
        ivec3 a = {1, 0, 0};
        ivec3 b = {-1, 0, 0};
        int result = ivec3_dot(&a, &b);
        REQUIRE(result == -1);
    }

    SECTION( "non-orthogonal vectors" ) {
        ivec3 a = {1, 0, 0};
        ivec3 b = {1, 1, 0};
        int result = ivec3_dot(&a, &b);
        REQUIRE(result == 1);
    }
}

TEST_CASE( "ivec3_cross", "[vec3]" ) {
    SECTION( "nan vector" ) {
        ivec3 nan = ivec3_nan();
        ivec3 v = {1, 0, 0};

        ivec3 result = ivec3_cross(&nan, &v);
        REQUIRE(ivec3_is_nan(&result) == true);

        result = ivec3_cross(&v, &nan);
        REQUIRE(ivec3_is_nan(&result) == true);
    }

    SECTION( "parallel vectors" ) {
        ivec3 a = {1, 0, 0};
        ivec3 b = {1, 0, 0};
        ivec3 result = ivec3_cross(&a, &b);
        REQUIRE(ivec3_is_zero(&result) == true);
    }

    SECTION( "perpendicular vectors" ) {
        ivec3 a = {1, 0, 0};
        ivec3 b = {0, 1, 0};
        ivec3 result = ivec3_cross(&a, &b);
        REQUIRE(result.x == 0);
        REQUIRE(result.y == 0);
        REQUIRE(result.z == 1);
    }

    SECTION( "non-orthogonal vectors" ) {
        ivec3 a = {1, 0, 0};
        ivec3 b = {0, 1, 0};
        ivec3 result = ivec3_cross(&a, &b);
        REQUIRE(result.x == 0);
        REQUIRE(result.y == 0);
        REQUIRE(result.z == 1);
    }
}
