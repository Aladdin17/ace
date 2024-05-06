#include <ace/math/math.h>
#include <ace/math/vec3_ext.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <math.h>

TEST_CASE( "ac_vec3_angle", "[ac_vec3]" ) {
    SECTION( "zero magnitude vectors" ) {
        ac_vec3 a = {0.0f, 0.0f, 0.0f};
        ac_vec3 b = {1.0f, 0.0f, 0.0f};

        // first vector is zero
        float angle = ac_rad_to_deg(ac_vec3_angle(&a, &b));
        REQUIRE( isnan(angle) );

        // second vector is zero
        angle = ac_rad_to_deg(ac_vec3_angle(&b, &a));
        REQUIRE( isnan(angle) );
    }

    SECTION( "nan vectors" ) {
        ac_vec3 a = {NAN, 0.0f, 0.0f};
        ac_vec3 b = {1.0f, 0.0f, 0.0f};

        // first vector is NaN
        float angle = ac_rad_to_deg(ac_vec3_angle(&a, &b));
        REQUIRE( isnan(angle) );

        // second vector is NaN
        angle = ac_rad_to_deg(ac_vec3_angle(&b, &a));
        REQUIRE( isnan(angle) );
    }

    SECTION( "90 degrees angle" ) {
        ac_vec3 a = {1.0f, 0.0f, 0.0f};
        ac_vec3 b = {0.0f, 1.0f, 0.0f};
        float result = ac_rad_to_deg(ac_vec3_angle(&a, &b));
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(90.0f, AC_EPSILON) ||
            Catch::Matchers::WithinAbs(90.0f, AC_EPSILON)
        );
    }

    SECTION( "45 degrees angle" ) {
        ac_vec3 a = {1.0f, 1.0f, 0.0f};
        ac_vec3 b = {1.0f, 0.0f, 0.0f};
        float result = ac_rad_to_deg(ac_vec3_angle(&a, &b));
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(45.0f, AC_EPSILON) ||
            Catch::Matchers::WithinAbs(45.0f, AC_EPSILON)
        );
    }

    SECTION( "0 degrees angle" ) {
        ac_vec3 a = {1.0f, 0.0f, 0.0f};
        ac_vec3 b = {1.0f, 0.0f, 0.0f};
        float result = ac_rad_to_deg(ac_vec3_angle(&a, &b));
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(0.0f, AC_EPSILON) ||
            Catch::Matchers::WithinAbs(0.0f, AC_EPSILON)
        );
    }

    SECTION( "-90 degrees angle" ) {
        ac_vec3 a = {1.0f, 0.0f, 0.0f};
        ac_vec3 b = {0.0f, -1.0f, 0.0f};
        float result = ac_rad_to_deg(ac_vec3_angle(&a, &b));
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(90.0f, AC_EPSILON) ||
            Catch::Matchers::WithinAbs(90.0f, AC_EPSILON)
        );
    }
}

TEST_CASE( "ac_vec3_distance", "[ac_vec3]" ) {
    SECTION( "nan vectors" ) {
        ac_vec3 nan = ac_vec3_nan();
        ac_vec3 v = {1.0f, 0.0f, 0.0f};

        // first vector is NaN
        float result = ac_vec3_distance(&nan, &v);
        REQUIRE(isnan(result));

        // second vector is NaN
        result = ac_vec3_distance(&v, &nan);
        REQUIRE(isnan(result));
    }

    SECTION( "zero vectors" ) {
        ac_vec3 zero = ac_vec3_zero();
        ac_vec3 v = {1.0f, 0.0f, 0.0f};

        // first vector is zero
        float result = ac_vec3_distance(&zero, &v);
        REQUIRE(result == 1.0f);

        // second vector is zero
        result = ac_vec3_distance(&v, &zero);
        REQUIRE(result == 1.0f);
    }

    SECTION( "non-zero vectors" ) {
        ac_vec3 a = {1.0f, 0.0f, 0.0f};
        ac_vec3 b = {0.0f, 1.0f, 0.0f};
        float result = ac_vec3_distance(&a, &b);
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(1.414214f, AC_EPSILON) ||
            Catch::Matchers::WithinAbs(1.414214f, AC_EPSILON)
        );
    }

    SECTION( "identical vectors" ) {
        ac_vec3 a = {1.0f, 0.0f, 0.0f};
        ac_vec3 b = {1.0f, 0.0f, 0.0f};
        float result = ac_vec3_distance(&a, &b);
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(0.0f, AC_EPSILON) ||
            Catch::Matchers::WithinAbs(0.0f, AC_EPSILON)
        );
    }
}

TEST_CASE( "ac_vec3_lerp", "ac_vec3" ) {
    ac_vec3 a = {1.0f, 0.0f, 0.0f};
    ac_vec3 b = {0.0f, 1.0f, 0.0f};
    ac_vec3 result;

    SECTION( "zero interpolation factor" ) {
        ac_vec3 expected = a;
        result = ac_vec3_lerp(&a, &b, 0.0f);
        REQUIRE(ac_vec3_is_equal(&result, &expected) == true);
    }

    SECTION( "full interpolation factor" ) {
        ac_vec3 expected = b;
        result = ac_vec3_lerp(&a, &b, 1.0f);
        REQUIRE(ac_vec3_is_equal(&result, &expected) == true);
    }

    SECTION( "half interpolation factor" ) {
        ac_vec3 expected = {0.5f, 0.5f, 0.0f};
        result = ac_vec3_lerp(&a, &b, 0.5f);
        REQUIRE(ac_vec3_is_equal(&result, &expected) == true);
    }

    SECTION( "negative interpolation factor" ) {
        ac_vec3 expected = a;
        result = ac_vec3_lerp(&a, &b, -0.5f);
        REQUIRE(ac_vec3_is_equal(&result, &expected) == true);
    }

    SECTION( "interpolation factor greater than 1" ) {
        ac_vec3 expected = b;
        result = ac_vec3_lerp(&a, &b, 2.0f);
        REQUIRE(ac_vec3_is_equal(&result, &expected) == true);
    }

    SECTION( "nan vectors" ) {
        ac_vec3 nan = ac_vec3_nan();
        result = ac_vec3_lerp(&nan, &b, 0.5f);
        REQUIRE(ac_vec3_is_nan(&result) == true);

        result = ac_vec3_lerp(&a, &nan, 0.5f);
        REQUIRE(ac_vec3_is_nan(&result) == true);
    }
}

TEST_CASE( "ac_vec3_project", "[ac_vec3]" ) {
    SECTION( "nan vectors" ) {
        ac_vec3 nan = ac_vec3_nan();
        ac_vec3 v = {1.0f, 0.0f, 0.0f};

        // first vector is NaN
        ac_vec3 result = ac_vec3_project(&nan, &v);
        REQUIRE(ac_vec3_is_nan(&result) == true);

        // second vector is NaN
        result = ac_vec3_project(&v, &nan);
        REQUIRE(ac_vec3_is_nan(&result) == true);
    }

    SECTION( "zero vectors" ) {
        ac_vec3 zero = ac_vec3_zero();
        ac_vec3 v = {1.0f, 0.0f, 0.0f};

        // first vector is zero
        ac_vec3 result = ac_vec3_project(&zero, &v);
        REQUIRE(ac_vec3_is_nan(&result) == true);

        // second vector is zero
        result = ac_vec3_project(&v, &zero);
        REQUIRE(ac_vec3_is_nan(&result) == true);
    }

    SECTION( "parallel vectors" ) {
        ac_vec3 a = {1.0f, 0.0f, 0.0f};
        ac_vec3 b = {1.0f, 0.0f, 0.0f};
        ac_vec3 result = ac_vec3_project(&a, &b);
        REQUIRE(ac_vec3_is_equal(&result, &a) == true);
    }

    SECTION( "perpendicular vectors" ) {
        ac_vec3 a = {1.0f, 0.0f, 0.0f};
        ac_vec3 b = {0.0f, 1.0f, 0.0f};
        ac_vec3 result = ac_vec3_project(&a, &b);
        REQUIRE(ac_vec3_is_zero(&result) == true);
    }

    SECTION( "oblique vectors" ) {
        ac_vec3 a = {1.0f, 1.0f, 0.0f};
        ac_vec3 b = {1.0f, 0.0f, 0.0f};
        ac_vec3 expected = {1.0f, 0.0f, 0.0f};
        ac_vec3 result = ac_vec3_project(&a, &b);
        REQUIRE(ac_vec3_is_equal(&result, &expected) == true);
    }
}

TEST_CASE( "ac_vec3_reflect", "[ac_vec3]" ) {
    SECTION( "nan vectors" ) {
        ac_vec3 nan = ac_vec3_nan();
        ac_vec3 v = {1.0f, 0.0f, 0.0f};

        // first vector is NaN
        ac_vec3 result = ac_vec3_reflect(&nan, &v);
        REQUIRE(ac_vec3_is_nan(&result) == true);

        // second vector is NaN
        result = ac_vec3_reflect(&v, &nan);
        REQUIRE(ac_vec3_is_nan(&result) == true);
    }

    SECTION( "zero vectors" ) {
        ac_vec3 zero = ac_vec3_zero();
        ac_vec3 v = {1.0f, 0.0f, 0.0f};

        // first vector is zero
        ac_vec3 result = ac_vec3_reflect(&zero, &v);
        REQUIRE(ac_vec3_is_nan(&result) == true);

        // second vector is zero
        result = ac_vec3_reflect(&v, &zero);
        REQUIRE(ac_vec3_is_nan(&result) == true);
    }

    SECTION( "perpendicular vector" ) {
        ac_vec3 v = {1.0f, 0.0f, 0.0f};
        ac_vec3 n = {0.0f, 1.0f, 0.0f};
        ac_vec3 expected = {1.0f, 0.0f, 0.0f};
        ac_vec3 result = ac_vec3_reflect(&v, &n);
        REQUIRE(ac_vec3_is_equal(&result, &expected) == true);
    }

    SECTION( "parallel vector" ) {
        ac_vec3 v = {1.0f, 0.0f, 0.0f};
        ac_vec3 n = {1.0f, 0.0f, 0.0f};
        ac_vec3 expected = {-1.0f, 0.0f, 0.0f};
        ac_vec3 result = ac_vec3_reflect(&v, &n);
        REQUIRE(ac_vec3_is_equal(&result, &expected) == true);
    }

    SECTION( "oblique vector" ) {
        ac_vec3 v = {1.0f, 1.0f, 0.0f};
        ac_vec3 n = {0.0f, 1.0f, 0.0f};
        ac_vec3 expected = {1.0f, -1.0f, 0.0f};
        ac_vec3 result = ac_vec3_reflect(&v, &n);
        REQUIRE(ac_vec3_is_equal(&result, &expected) == true);
    }
}

TEST_CASE( "vec3_mult_matrix", "[vec3]" ) {
    SECTION( "nan arguments" ) {
        ac_vec3 nan_vec = ac_vec3_nan();
        ac_mat3 nan_mat = ac_mat3_nan();
        ac_vec3 vec = {1.0f, 1.0f, 1.0f};
        ac_mat3 mat = ac_mat3_identity();

        // vec is nan
        ac_vec3 result = ac_vec3_mult_matrix(&nan_vec, &mat);
        REQUIRE(ac_vec3_is_nan(&result) == true);

        // mat is nan
        result = ac_vec3_mult_matrix(&vec, &nan_mat);
        REQUIRE(ac_vec3_is_nan(&result) == true);
    }

    SECTION( "identity matrix" ) {
        ac_vec3 vec = {1.0f, 2.0f, 3.0f};
        ac_mat3 mat = ac_mat3_identity();
        ac_vec3 expected = vec;
        ac_vec3 result = ac_vec3_mult_matrix(&vec, &mat);
        REQUIRE(ac_vec3_is_equal(&result, &expected) == true);
    }

    SECTION( "zero matrix" ) {
        ac_vec3 vec = {1.0f, 2.0f, 3.0f};
        ac_mat3 mat = ac_mat3_zero();
        ac_vec3 expected = ac_vec3_zero();
        ac_vec3 result = ac_vec3_mult_matrix(&vec, &mat);
        REQUIRE(ac_vec3_is_equal(&result, &expected) == true);
    }

    SECTION( "scaling matrix" ) {
        ac_vec3 vec = {1.0f, 2.0f, 3.0f};
        ac_mat3 mat = ac_mat3_identity();
        mat.m00 = 2.0f;
        mat.m11 = 2.0f;
        mat.m22 = 2.0f;
        ac_vec3 expected = {2.0f, 4.0f, 6.0f};
        ac_vec3 result = ac_vec3_mult_matrix(&vec, &mat);
        REQUIRE(ac_vec3_is_equal(&result, &expected) == true);
    }
}
