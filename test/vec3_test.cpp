#include <alibrary/math/vec3.h>
#include <catch2/catch_assertion_result.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/matchers/catch_matchers_predicate.hpp>
#include <tuple>

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

TEST_CASE( "vec3_angle", "[vec3]" ) {
    SECTION( "zero magnitude vectors" ) {
        vec3 a = {0.0f, 0.0f, 0.0f};
        vec3 b = {1.0f, 0.0f, 0.0f};

        // first vector is zero
        float angle = al_rad_to_deg(vec3_angle(&a, &b));
        REQUIRE( isnan(angle) );

        // second vector is zero
        angle = al_rad_to_deg(vec3_angle(&b, &a));
        REQUIRE( isnan(angle) );
    }

    SECTION( "nan vectors" ) {
        vec3 a = {NAN, 0.0f, 0.0f};
        vec3 b = {1.0f, 0.0f, 0.0f};

        // first vector is NaN
        float angle = al_rad_to_deg(vec3_angle(&a, &b));
        REQUIRE( isnan(angle) );

        // second vector is NaN
        angle = al_rad_to_deg(vec3_angle(&b, &a));
        REQUIRE( isnan(angle) );
    }

    SECTION( "90 degrees angle" ) {
        vec3 a = {1.0f, 0.0f, 0.0f};
        vec3 b = {0.0f, 1.0f, 0.0f};
        float result = al_rad_to_deg(vec3_angle(&a, &b));
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(90.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(90.0f, AL_EPSILON)
        );
    }

    SECTION( "45 degrees angle" ) {
        vec3 a = {1.0f, 1.0f, 0.0f};
        vec3 b = {1.0f, 0.0f, 0.0f};
        float result = al_rad_to_deg(vec3_angle(&a, &b));
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(45.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(45.0f, AL_EPSILON)
        );
    }

    SECTION( "0 degrees angle" ) {
        vec3 a = {1.0f, 0.0f, 0.0f};
        vec3 b = {1.0f, 0.0f, 0.0f};
        float result = al_rad_to_deg(vec3_angle(&a, &b));
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(0.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(0.0f, AL_EPSILON)
        );
    }

    SECTION( "-90 degrees angle" ) {
        vec3 a = {1.0f, 0.0f, 0.0f};
        vec3 b = {0.0f, -1.0f, 0.0f};
        float result = al_rad_to_deg(vec3_angle(&a, &b));
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(90.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(90.0f, AL_EPSILON)
        );
    }
}

TEST_CASE( "vec3_reflect", "[vec3]" ) {
    SECTION( "nan vectors" ) {
        vec3 nan = vec3_nan();
        vec3 v = {1.0f, 0.0f, 0.0f};

        // first vector is NaN
        vec3 result = vec3_reflect(&nan, &v);
        REQUIRE(vec3_is_nan(&result) == true);

        // second vector is NaN
        result = vec3_reflect(&v, &nan);
        REQUIRE(vec3_is_nan(&result) == true);
    }

    SECTION( "zero vectors" ) {
        vec3 zero = vec3_zero();
        vec3 v = {1.0f, 0.0f, 0.0f};

        // first vector is zero
        vec3 result = vec3_reflect(&zero, &v);
        REQUIRE(vec3_is_nan(&result) == true);

        // second vector is zero
        result = vec3_reflect(&v, &zero);
        REQUIRE(vec3_is_nan(&result) == true);
    }

    SECTION( "perpendicular vector" ) {
        vec3 v = {1.0f, 0.0f, 0.0f};
        vec3 n = {0.0f, 1.0f, 0.0f};
        vec3 expected = {1.0f, 0.0f, 0.0f};
        vec3 result = vec3_reflect(&v, &n);
        REQUIRE(vec3_is_equal(&result, &expected) == true);
    }

    SECTION( "parallel vector" ) {
        vec3 v = {1.0f, 0.0f, 0.0f};
        vec3 n = {1.0f, 0.0f, 0.0f};
        vec3 expected = {-1.0f, 0.0f, 0.0f};
        vec3 result = vec3_reflect(&v, &n);
        REQUIRE(vec3_is_equal(&result, &expected) == true);
    }

    SECTION( "oblique vector" ) {
        vec3 v = {1.0f, 1.0f, 0.0f};
        vec3 n = {0.0f, 1.0f, 0.0f};
        vec3 expected = {1.0f, -1.0f, 0.0f};
        vec3 result = vec3_reflect(&v, &n);
        REQUIRE(vec3_is_equal(&result, &expected) == true);
    }
}

TEST_CASE( "vec3_distance", "[vec3]" ) {
    SECTION( "nan vectors" ) {
        vec3 nan = vec3_nan();
        vec3 v = {1.0f, 0.0f, 0.0f};

        // first vector is NaN
        float result = vec3_distance(&nan, &v);
        REQUIRE(isnan(result));

        // second vector is NaN
        result = vec3_distance(&v, &nan);
        REQUIRE(isnan(result));
    }

    SECTION( "zero vectors" ) {
        vec3 zero = vec3_zero();
        vec3 v = {1.0f, 0.0f, 0.0f};

        // first vector is zero
        float result = vec3_distance(&zero, &v);
        REQUIRE(result == 1.0f);

        // second vector is zero
        result = vec3_distance(&v, &zero);
        REQUIRE(result == 1.0f);
    }

    SECTION( "non-zero vectors" ) {
        vec3 a = {1.0f, 0.0f, 0.0f};
        vec3 b = {0.0f, 1.0f, 0.0f};
        float result = vec3_distance(&a, &b);
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(1.414214f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(1.414214f, AL_EPSILON)
        );
    }

    SECTION( "identical vectors" ) {
        vec3 a = {1.0f, 0.0f, 0.0f};
        vec3 b = {1.0f, 0.0f, 0.0f};
        float result = vec3_distance(&a, &b);
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(0.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(0.0f, AL_EPSILON)
        );
    }
}

TEST_CASE( "vec3_lerp", "vec3" ) {
    vec3 a = {1.0f, 0.0f, 0.0f};
    vec3 b = {0.0f, 1.0f, 0.0f};
    vec3 result;

    SECTION( "zero interpolation factor" ) {
        vec3 expected = a;
        result = vec3_lerp(&a, &b, 0.0f);
        REQUIRE(vec3_is_equal(&result, &expected) == true);
    }

    SECTION( "full interpolation factor" ) {
        vec3 expected = b;
        result = vec3_lerp(&a, &b, 1.0f);
        REQUIRE(vec3_is_equal(&result, &expected) == true);
    }

    SECTION( "half interpolation factor" ) {
        vec3 expected = {0.5f, 0.5f, 0.0f};
        result = vec3_lerp(&a, &b, 0.5f);
        REQUIRE(vec3_is_equal(&result, &expected) == true);
    }

    SECTION( "negative interpolation factor" ) {
        vec3 expected = a;
        result = vec3_lerp(&a, &b, -0.5f);
        REQUIRE(vec3_is_equal(&result, &expected) == true);
    }

    SECTION( "interpolation factor greater than 1" ) {
        vec3 expected = b;
        result = vec3_lerp(&a, &b, 2.0f);
        REQUIRE(vec3_is_equal(&result, &expected) == true);
    }

    SECTION( "nan vectors" ) {
        vec3 nan = vec3_nan();
        result = vec3_lerp(&nan, &b, 0.5f);
        REQUIRE(vec3_is_nan(&result) == true);

        result = vec3_lerp(&a, &nan, 0.5f);
        REQUIRE(vec3_is_nan(&result) == true);
    }
}

TEST_CASE( "vec3_project", "[vec3]" ) {
    SECTION( "nan vectors" ) {
        vec3 nan = vec3_nan();
        vec3 v = {1.0f, 0.0f, 0.0f};

        // first vector is NaN
        vec3 result = vec3_project(&nan, &v);
        REQUIRE(vec3_is_nan(&result) == true);

        // second vector is NaN
        result = vec3_project(&v, &nan);
        REQUIRE(vec3_is_nan(&result) == true);
    }

    SECTION( "zero vectors" ) {
        vec3 zero = vec3_zero();
        vec3 v = {1.0f, 0.0f, 0.0f};

        // first vector is zero
        vec3 result = vec3_project(&zero, &v);
        REQUIRE(vec3_is_nan(&result) == true);

        // second vector is zero
        result = vec3_project(&v, &zero);
        REQUIRE(vec3_is_nan(&result) == true);
    }

    SECTION( "parallel vectors" ) {
        vec3 a = {1.0f, 0.0f, 0.0f};
        vec3 b = {1.0f, 0.0f, 0.0f};
        vec3 result = vec3_project(&a, &b);
        REQUIRE(vec3_is_equal(&result, &a) == true);
    }

    SECTION( "perpendicular vectors" ) {
        vec3 a = {1.0f, 0.0f, 0.0f};
        vec3 b = {0.0f, 1.0f, 0.0f};
        vec3 result = vec3_project(&a, &b);
        REQUIRE(vec3_is_zero(&result) == true);
    }

    SECTION( "oblique vectors" ) {
        vec3 a = {1.0f, 1.0f, 0.0f};
        vec3 b = {1.0f, 0.0f, 0.0f};
        vec3 expected = {1.0f, 0.0f, 0.0f};
        vec3 result = vec3_project(&a, &b);
        REQUIRE(vec3_is_equal(&result, &expected) == true);
    }
}
