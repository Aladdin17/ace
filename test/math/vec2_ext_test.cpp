#include <ace/math/math.h>
#include <ace/math/vec2_ext.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <math.h>

TEST_CASE( "ac_vec2_angle", "[ac_vec2]" ) {
    SECTION( "zero magnitude vectors" ) {
        ac_vec2 a = {0.0f, 0.0f};
        ac_vec2 b = {1.0f, 0.0f};

        // first vector is zero
        float angle = ac_rad_to_deg(ac_vec2_angle(&a, &b));
        REQUIRE( isnan(angle) );

        // second vector is zero
        angle = ac_rad_to_deg(ac_vec2_angle(&b, &a));
        REQUIRE( isnan(angle) );
    }

    SECTION( "nan vectors" ) {
        ac_vec2 a = {NAN, 0.0f};
        ac_vec2 b = {1.0f, 0.0f};

        // first vector is NaN
        float angle = ac_rad_to_deg(ac_vec2_angle(&a, &b));
        REQUIRE( isnan(angle) );

        // second vector is NaN
        angle = ac_rad_to_deg(ac_vec2_angle(&b, &a));
        REQUIRE( isnan(angle) );
    }

    SECTION( "90 degrees angle" ) {
        ac_vec2 a = {1.0f, 0.0f};
        ac_vec2 b = {0.0f, 1.0f};
        float result = ac_rad_to_deg(ac_vec2_angle(&a, &b));
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(90.0f, AC_EPSILON) ||
            Catch::Matchers::WithinAbs(90.0f, AC_EPSILON)
        );
    }

    SECTION( "45 degrees angle" ) {
        ac_vec2 a = {1.0f, 1.0f};
        ac_vec2 b = {1.0f, 0.0f};
        float result = ac_rad_to_deg(ac_vec2_angle(&a, &b));
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(45.0f, AC_EPSILON) ||
            Catch::Matchers::WithinAbs(45.0f, AC_EPSILON)
        );
    }

    SECTION( "0 degrees angle" ) {
        ac_vec2 a = {1.0f, 0.0f};
        ac_vec2 b = {1.0f, 0.0f};
        float result = ac_rad_to_deg(ac_vec2_angle(&a, &b));
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(0.0f, AC_EPSILON) ||
            Catch::Matchers::WithinAbs(0.0f, AC_EPSILON)
        );
    }

    SECTION( "-90 degrees angle" ) {
        ac_vec2 a = {1.0f, 0.0f};
        ac_vec2 b = {0.0f, -1.0f};
        float result = ac_rad_to_deg(ac_vec2_angle(&a, &b));
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(90.0f, AC_EPSILON) ||
            Catch::Matchers::WithinAbs(90.0f, AC_EPSILON)
        );
    }
}

TEST_CASE( "ac_vec2_distance", "[ac_vec2]" ) {
    SECTION( "nan vectors" ) {
        ac_vec2 nan = ac_vec2_nan();
        ac_vec2 v = {1.0f, 0.0f};

        // first vector is NaN
        float result = ac_vec2_distance(&nan, &v);
        REQUIRE(isnan(result));

        // second vector is NaN
        result = ac_vec2_distance(&v, &nan);
        REQUIRE(isnan(result));
    }

    SECTION( "zero vectors" ) {
        ac_vec2 zero = ac_vec2_zero();
        ac_vec2 v = {1.0f, 0.0f};

        // first vector is zero
        float result = ac_vec2_distance(&zero, &v);
        REQUIRE(result == 1.0f);

        // second vector is zero
        result = ac_vec2_distance(&v, &zero);
        REQUIRE(result == 1.0f);
    }

    SECTION( "non-zero vectors" ) {
        ac_vec2 a = {1.0f, 0.0f};
        ac_vec2 b = {0.0f, 1.0f};
        float result = ac_vec2_distance(&a, &b);
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(1.414214f, AC_EPSILON) ||
            Catch::Matchers::WithinAbs(1.414214f, AC_EPSILON)
        );
    }

    SECTION( "identical vectors" ) {
        ac_vec2 a = {1.0f, 0.0f};
        ac_vec2 b = {1.0f, 0.0f};
        float result = ac_vec2_distance(&a, &b);
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(0.0f, AC_EPSILON) ||
            Catch::Matchers::WithinAbs(0.0f, AC_EPSILON)
        );
    }
}

TEST_CASE( "ac_vec2_lerp", "ac_vec2" ) {
    ac_vec2 a = {1.0f, 0.0f};
    ac_vec2 b = {0.0f, 1.0f};
    ac_vec2 result;

    SECTION( "zero interpolation factor" ) {
        ac_vec2 expected = a;
        result = ac_vec2_lerp(&a, &b, 0.0f);
        REQUIRE(ac_vec2_is_equal(&result, &expected) == true);
    }

    SECTION( "full interpolation factor" ) {
        ac_vec2 expected = b;
        result = ac_vec2_lerp(&a, &b, 1.0f);
        REQUIRE(ac_vec2_is_equal(&result, &expected) == true);
    }

    SECTION( "half interpolation factor" ) {
        ac_vec2 expected = {0.5f, 0.5f};
        result = ac_vec2_lerp(&a, &b, 0.5f);
        REQUIRE(ac_vec2_is_equal(&result, &expected) == true);
    }

    SECTION( "negative interpolation factor" ) {
        ac_vec2 expected = a;
        result = ac_vec2_lerp(&a, &b, -0.5f);
        REQUIRE(ac_vec2_is_equal(&result, &expected) == true);
    }

    SECTION( "interpolation factor greater than 1" ) {
        ac_vec2 expected = b;
        result = ac_vec2_lerp(&a, &b, 2.0f);
        REQUIRE(ac_vec2_is_equal(&result, &expected) == true);
    }

    SECTION( "nan vectors" ) {
        ac_vec2 nan = ac_vec2_nan();
        result = ac_vec2_lerp(&nan, &b, 0.5f);
        REQUIRE(ac_vec2_is_nan(&result) == true);

        result = ac_vec2_lerp(&a, &nan, 0.5f);
        REQUIRE(ac_vec2_is_nan(&result) == true);
    }
}

TEST_CASE( "ac_vec2_project", "[ac_vec2]" ) {
    SECTION( "nan vectors" ) {
        ac_vec2 nan = ac_vec2_nan();
        ac_vec2 v = {1.0f, 0.0f};

        // first vector is NaN
        ac_vec2 result = ac_vec2_project(&nan, &v);
        REQUIRE(ac_vec2_is_nan(&result) == true);

        // second vector is NaN
        result = ac_vec2_project(&v, &nan);
        REQUIRE(ac_vec2_is_nan(&result) == true);
    }

    SECTION( "zero vectors" ) {
        ac_vec2 zero = ac_vec2_zero();
        ac_vec2 v = {1.0f, 0.0f};

        // first vector is zero
        ac_vec2 result = ac_vec2_project(&zero, &v);
        REQUIRE(ac_vec2_is_nan(&result) == true);

        // second vector is zero
        result = ac_vec2_project(&v, &zero);
        REQUIRE(ac_vec2_is_nan(&result) == true);
    }

    SECTION( "parallel vectors" ) {
        ac_vec2 a = {1.0f, 0.0f};
        ac_vec2 b = {1.0f, 0.0f};
        ac_vec2 result = ac_vec2_project(&a, &b);
        REQUIRE(ac_vec2_is_equal(&result, &a) == true);
    }

    SECTION( "perpendicular vectors" ) {
        ac_vec2 a = {1.0f, 0.0f};
        ac_vec2 b = {0.0f, 1.0f};
        ac_vec2 result = ac_vec2_project(&a, &b);
        REQUIRE(ac_vec2_is_zero(&result) == true);
    }

    SECTION( "oblique vectors" ) {
        ac_vec2 a = {1.0f, 1.0f};
        ac_vec2 b = {1.0f, 0.0f};
        ac_vec2 expected = {1.0f, 0.0f};
        ac_vec2 result = ac_vec2_project(&a, &b);
        REQUIRE(ac_vec2_is_equal(&result, &expected) == true);
    }
}

TEST_CASE( "ac_vec2_reflect", "[ac_vec2]" ) {
    SECTION( "nan vectors" ) {
        ac_vec2 nan = ac_vec2_nan();
        ac_vec2 v = {1.0f, 0.0f};

        // first vector is NaN
        ac_vec2 result = ac_vec2_reflect(&nan, &v);
        REQUIRE(ac_vec2_is_nan(&result) == true);

        // second vector is NaN
        result = ac_vec2_reflect(&v, &nan);
        REQUIRE(ac_vec2_is_nan(&result) == true);
    }

    SECTION( "zero vectors" ) {
        ac_vec2 zero = ac_vec2_zero();
        ac_vec2 v = {1.0f, 0.0f};

        // first vector is zero
        ac_vec2 result = ac_vec2_reflect(&zero, &v);
        REQUIRE(ac_vec2_is_nan(&result) == true);

        // second vector is zero
        result = ac_vec2_reflect(&v, &zero);
        REQUIRE(ac_vec2_is_nan(&result) == true);
    }

    SECTION( "perpendicular vector" ) {
        ac_vec2 v = {1.0f, 0.0f};
        ac_vec2 n = {0.0f, 1.0f};
        ac_vec2 expected = {1.0f, 0.0f};
        ac_vec2 result = ac_vec2_reflect(&v, &n);
        REQUIRE(ac_vec2_is_equal(&result, &expected) == true);
    }

    SECTION( "parallel vector" ) {
        ac_vec2 v = {1.0f, 0.0f};
        ac_vec2 n = {1.0f, 0.0f};
        ac_vec2 expected = {-1.0f, 0.0f};
        ac_vec2 result = ac_vec2_reflect(&v, &n);
        REQUIRE(ac_vec2_is_equal(&result, &expected) == true);
    }

    SECTION( "oblique vector" ) {
        ac_vec2 v = {1.0f, 1.0f};
        ac_vec2 n = {0.0f, 1.0f};
        ac_vec2 expected = {1.0f, -1.0f};
        ac_vec2 result = ac_vec2_reflect(&v, &n);
        REQUIRE(ac_vec2_is_equal(&result, &expected) == true);
    }
}
