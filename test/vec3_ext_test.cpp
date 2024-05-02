#include <alibrary/math/math.h>
#include <alibrary/math/vec3_ext.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <math.h>

TEST_CASE( "vec3_angle", "[vec3]" ) {
    SECTION( "zero magnitude vectors" ) {
        vec3 a = {0.0f, 0.0f, 0.0f};
        vec3 b = {1.0f, 0.0f, 0.0f};

        // first vector is zero
        float angle = ac_rad_to_deg(vec3_angle(&a, &b));
        REQUIRE( isnan(angle) );

        // second vector is zero
        angle = ac_rad_to_deg(vec3_angle(&b, &a));
        REQUIRE( isnan(angle) );
    }

    SECTION( "nan vectors" ) {
        vec3 a = {NAN, 0.0f, 0.0f};
        vec3 b = {1.0f, 0.0f, 0.0f};

        // first vector is NaN
        float angle = ac_rad_to_deg(vec3_angle(&a, &b));
        REQUIRE( isnan(angle) );

        // second vector is NaN
        angle = ac_rad_to_deg(vec3_angle(&b, &a));
        REQUIRE( isnan(angle) );
    }

    SECTION( "90 degrees angle" ) {
        vec3 a = {1.0f, 0.0f, 0.0f};
        vec3 b = {0.0f, 1.0f, 0.0f};
        float result = ac_rad_to_deg(vec3_angle(&a, &b));
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(90.0f, AC_EPSILON) ||
            Catch::Matchers::WithinAbs(90.0f, AC_EPSILON)
        );
    }

    SECTION( "45 degrees angle" ) {
        vec3 a = {1.0f, 1.0f, 0.0f};
        vec3 b = {1.0f, 0.0f, 0.0f};
        float result = ac_rad_to_deg(vec3_angle(&a, &b));
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(45.0f, AC_EPSILON) ||
            Catch::Matchers::WithinAbs(45.0f, AC_EPSILON)
        );
    }

    SECTION( "0 degrees angle" ) {
        vec3 a = {1.0f, 0.0f, 0.0f};
        vec3 b = {1.0f, 0.0f, 0.0f};
        float result = ac_rad_to_deg(vec3_angle(&a, &b));
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(0.0f, AC_EPSILON) ||
            Catch::Matchers::WithinAbs(0.0f, AC_EPSILON)
        );
    }

    SECTION( "-90 degrees angle" ) {
        vec3 a = {1.0f, 0.0f, 0.0f};
        vec3 b = {0.0f, -1.0f, 0.0f};
        float result = ac_rad_to_deg(vec3_angle(&a, &b));
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(90.0f, AC_EPSILON) ||
            Catch::Matchers::WithinAbs(90.0f, AC_EPSILON)
        );
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
            Catch::Matchers::WithinRel(1.414214f, AC_EPSILON) ||
            Catch::Matchers::WithinAbs(1.414214f, AC_EPSILON)
        );
    }

    SECTION( "identical vectors" ) {
        vec3 a = {1.0f, 0.0f, 0.0f};
        vec3 b = {1.0f, 0.0f, 0.0f};
        float result = vec3_distance(&a, &b);
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(0.0f, AC_EPSILON) ||
            Catch::Matchers::WithinAbs(0.0f, AC_EPSILON)
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
