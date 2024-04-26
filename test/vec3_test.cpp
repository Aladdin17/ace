#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_assertion_result.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/matchers/catch_matchers_predicate.hpp>
#include <alibrary/math/vec3.h>

TEST_CASE("Vec3 operations", "[vec3]") {
    Vec3 a = {1.0f, 2.0f, 3.0f};
    Vec3 b = {4.0f, 5.0f, 6.0f};

    SECTION("vec3_zero") {
        Vec3 zero_vec = vec3_zero();
        REQUIRE_THAT(zero_vec.x,
            Catch::Matchers::WithinRel(0.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(0.0f, AL_EPSILON));
        REQUIRE_THAT(zero_vec.y,
            Catch::Matchers::WithinRel(0.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(0.0f, AL_EPSILON));
        REQUIRE_THAT(zero_vec.z,
            Catch::Matchers::WithinRel(0.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(0.0f, AL_EPSILON));
    }

    SECTION("vec3_nan") {
        Vec3 nan_vec = vec3_nan();
        REQUIRE_THAT(nan_vec.x,
            Catch::Matchers::Predicate<float>([](float x) { return isnan(x); }, "NaN")
        );
        REQUIRE_THAT(nan_vec.y,
            Catch::Matchers::Predicate<float>([](float x) { return isnan(x); }, "NaN")
        );
        REQUIRE_THAT(nan_vec.z,
            Catch::Matchers::Predicate<float>([](float x) { return isnan(x); }, "NaN")
        );
    }

    SECTION("vec3_is_zero") {
        Vec3 zero_vec = { 0.0f, 0.0f, 0.0f };
        Vec3 non_zero_vec = { 0.0f, 0.0f, 1.0f };
        Vec3 negative_non_zero_vec = { 0.0f, 0.0f, -1.0f };
        REQUIRE(vec3_is_zero(&zero_vec) == true);
        REQUIRE(vec3_is_zero(&non_zero_vec) == false);
        REQUIRE(vec3_is_zero(&negative_non_zero_vec) == false);
    }

    SECTION("vec3_is_nan") {
        Vec3 nan_1 = {NAN, 0.0f, 0.0f};
        Vec3 nan_2 = {0.0f, NAN, 0.0f};
        Vec3 nan_3 = {0.0f, 0.0f, NAN};
        REQUIRE(vec3_is_nan(&nan_1) == true);
        REQUIRE(vec3_is_nan(&nan_2) == true);
        REQUIRE(vec3_is_nan(&nan_3) == true);
        REQUIRE(vec3_is_nan(&a) == false);
    }


    SECTION("vec3_add") {
        Vec3 result = vec3_add(&a, &b);
        REQUIRE_THAT(result.x,
            Catch::Matchers::WithinRel(5.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(5.0f, AL_EPSILON)
        );
        REQUIRE_THAT(result.y,
            Catch::Matchers::WithinRel(7.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(7.0f, AL_EPSILON)
        );
        REQUIRE_THAT(result.z,
            Catch::Matchers::WithinRel(9.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(9.0f, AL_EPSILON)
        );

        Vec3 nan = vec3_nan();
        result = vec3_add(&a, &nan);
        REQUIRE_THAT(result,
            Catch::Matchers::Predicate<Vec3>([](Vec3 v) { return vec3_is_nan(&v); }, "NaN")
        );
    }

    SECTION("vec3_sub") {
        Vec3 result = vec3_sub(&a, &b);
        REQUIRE_THAT(result.x,
            Catch::Matchers::WithinRel(-3.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(-3.0f, AL_EPSILON)
        );
        REQUIRE_THAT(result.y,
            Catch::Matchers::WithinRel(-3.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(-3.0f, AL_EPSILON)
        );
        REQUIRE_THAT(result.z,
            Catch::Matchers::WithinRel(-3.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(-3.0f, AL_EPSILON)
        );

        Vec3 nan = vec3_nan();
        result = vec3_sub(&a, &nan);
        REQUIRE_THAT(result,
            Catch::Matchers::Predicate<Vec3>([](Vec3 v) { return vec3_is_nan(&v); }, "NaN")
        );
    }

    SECTION("vec3_negate") {
        Vec3 result = vec3_negate(&a);
        REQUIRE_THAT(result.x,
            Catch::Matchers::WithinRel(-1.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(-1.0f, AL_EPSILON)
        );
        REQUIRE_THAT(result.y,
            Catch::Matchers::WithinRel(-2.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(-2.0f, AL_EPSILON)
        );
        REQUIRE_THAT(result.z,
            Catch::Matchers::WithinRel(-3.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(-3.0f, AL_EPSILON)
        );

        Vec3 nan = vec3_nan();
        result = vec3_negate(&nan);
        REQUIRE_THAT(result,
            Catch::Matchers::Predicate<Vec3>([](Vec3 v) { return vec3_is_nan(&v); }, "NaN")
        );

        Vec3 zero = vec3_zero();
        result = vec3_negate(&zero);
        REQUIRE_THAT(result,
            Catch::Matchers::Predicate<Vec3>([](Vec3 v) { return vec3_is_zero(&v); }, "Zero")
        );
    }

    SECTION("vec3_scale") {
        Vec3 result = vec3_scale(&a, 2.0f);
        REQUIRE_THAT(result.x,
            Catch::Matchers::WithinRel(2.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(2.0f, AL_EPSILON)
        );
        REQUIRE_THAT(result.y,
            Catch::Matchers::WithinRel(4.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(4.0f, AL_EPSILON)
        );
        REQUIRE_THAT(result.z,
            Catch::Matchers::WithinRel(6.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(6.0f, AL_EPSILON)
        );

        Vec3 nan = vec3_nan();
        result = vec3_scale(&nan, 2.0f);
        REQUIRE_THAT(result,
            Catch::Matchers::Predicate<Vec3>([](Vec3 v) { return vec3_is_nan(&v); }, "NaN")
        );

        Vec3 zero = vec3_zero();
        result = vec3_scale(&zero, 2.0f);
        REQUIRE_THAT(result,
            Catch::Matchers::Predicate<Vec3>([](Vec3 v) { return vec3_is_zero(&v); }, "Zero")
        );
    }

    SECTION("vec3_dot") {
        float result;

        Vec3 nan = vec3_nan();
        result = vec3_dot(&a, &nan);
        REQUIRE_THAT(result,
            Catch::Matchers::Predicate<float>([](float x) { return isnan(x); }, "NaN")
        );

        Vec3 zero = vec3_zero();
        result = vec3_dot(&a, &zero);
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(0.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(0.0f, AL_EPSILON)
        );

        result = vec3_dot(&a, &b);
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(32.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(32.0f, AL_EPSILON)
        );
    }

    SECTION("vec3_cross") {
        Vec3 result = vec3_cross(&a, &b);
        REQUIRE_THAT(result.x,
            Catch::Matchers::WithinRel(-3.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(-3.0f, AL_EPSILON)
        );
        REQUIRE_THAT(result.y,
            Catch::Matchers::WithinRel(6.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(6.0f, AL_EPSILON)
        );
        REQUIRE_THAT(result.z,
            Catch::Matchers::WithinRel(-3.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(-3.0f, AL_EPSILON)
        );

        Vec3 nan = vec3_nan();
        result = vec3_cross(&a, &nan);
        REQUIRE_THAT(result,
            Catch::Matchers::Predicate<Vec3>([](Vec3 v) { return vec3_is_nan(&v); }, "NaN")
        );

        Vec3 zero = vec3_zero();
        result = vec3_cross(&a, &zero);
        REQUIRE_THAT(result,
            Catch::Matchers::Predicate<Vec3>([](Vec3 v) { return vec3_is_zero(&v); }, "Zero")
        );
    }

    SECTION("vec3_magnitude") {
        float result = vec3_magnitude(&a);
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(3.741657f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(3.741657f, AL_EPSILON)
        );
    }

    SECTION("vec3_normalize") {
        Vec3 result = vec3_normalize(&a);
        REQUIRE_THAT(result.x,
            Catch::Matchers::WithinRel(0.267261f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(0.267261f, AL_EPSILON)
        );
        REQUIRE_THAT(result.y,
            Catch::Matchers::WithinRel(0.534522f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(0.534522f, AL_EPSILON)
        );
        REQUIRE_THAT(result.z,
            Catch::Matchers::WithinRel(0.801784f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(0.801784f, AL_EPSILON)
        );
    }
}

TEST_CASE("vec3 utilities angle", "[Vec3]") {
    SECTION("90 degrees angle") {
        Vec3 a = {1.0f, 0.0f, 0.0f};
        Vec3 b = {0.0f, 1.0f, 0.0f};
        float result = al_rad_to_deg(vec3_angle(&a, &b));
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(90.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(90.0f, AL_EPSILON)
        );
    }

    SECTION( "45 degrees angle" ) {
        Vec3 a = {1.0f, 1.0f, 0.0f};
        Vec3 b = {1.0f, 0.0f, 0.0f};
        float result = al_rad_to_deg(vec3_angle(&a, &b));
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(45.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(45.0f, AL_EPSILON)
        );
    }

    SECTION( "0 degrees angle" ) {
        Vec3 a = {1.0f, 0.0f, 0.0f};
        Vec3 b = {1.0f, 0.0f, 0.0f};
        float result = al_rad_to_deg(vec3_angle(&a, &b));
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(0.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(0.0f, AL_EPSILON)
        );
    }

    SECTION( "'-90' degrees angle") {
        Vec3 a = {1.0f, 0.0f, 0.0f};
        Vec3 b = {0.0f, -1.0f, 0.0f};
        float result = al_rad_to_deg(vec3_angle(&a, &b));
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(90.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(90.0f, AL_EPSILON)
        );
    }

    SECTION( "Zero magnitude vector") {
        Vec3 a = {0.0f, 0.0f, 0.0f};
        Vec3 b = {1.0f, 0.0f, 0.0f};
        float result = al_rad_to_deg(vec3_angle(&a, &b));
        REQUIRE_THAT(result,
            Catch::Matchers::Predicate<float>([](float x) { return isnan(x); }, "NaN")
        );
    }
}

TEST_CASE("vec3 utilities reflect", "[Vec3]") {
    SECTION("Reflect a perpendicular vector") {
        Vec3 v = {1.0f, 0.0f, 0.0f};
        Vec3 normal = {0.0f, 1.0f, 0.0f};
        Vec3 result = vec3_reflect(&v, &normal);
        REQUIRE_THAT(result.x, Catch::Matchers::WithinRel(1.0f, AL_EPSILON) ||
                              Catch::Matchers::WithinAbs(1.0f, AL_EPSILON));
        REQUIRE_THAT(result.y, Catch::Matchers::WithinRel(0.0f, AL_EPSILON) ||
                              Catch::Matchers::WithinAbs(0.0f, AL_EPSILON));
        REQUIRE_THAT(result.z, Catch::Matchers::WithinRel(0.0f, AL_EPSILON) ||
                              Catch::Matchers::WithinAbs(0.0f, AL_EPSILON));
    }

    SECTION("Reflect a parallel vector") {
        Vec3 v = {1.0f, 0.0f, 0.0f};
        Vec3 normal = {1.0f, 0.0f, 0.0f};
        Vec3 result = vec3_reflect(&v, &normal);
        REQUIRE_THAT(result.x, Catch::Matchers::WithinRel(-1.0f, AL_EPSILON) ||
                              Catch::Matchers::WithinAbs(-1.0f, AL_EPSILON));
        REQUIRE_THAT(result.y, Catch::Matchers::WithinRel(0.0f, AL_EPSILON) ||
                              Catch::Matchers::WithinAbs(0.0f, AL_EPSILON));
        REQUIRE_THAT(result.z, Catch::Matchers::WithinRel(0.0f, AL_EPSILON) ||
                              Catch::Matchers::WithinAbs(0.0f, AL_EPSILON));
    }

    SECTION("Reflecting a vector") {
        Vec3 v = {1.0f, 1.0f, 0.0f};
        Vec3 normal = {0.0f, 1.0f, 0.0f};
        Vec3 result = vec3_reflect(&v, &normal);
        REQUIRE_THAT(result.x,
            Catch::Matchers::WithinRel(1.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(1.0f, AL_EPSILON)
        );
        REQUIRE_THAT(result.y,
            Catch::Matchers::WithinRel(-1.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(-1.0f, AL_EPSILON)
        );
        REQUIRE_THAT(result.z,
            Catch::Matchers::WithinRel(0.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(0.0f, AL_EPSILON)
        );
    }

    SECTION("Reflect a zero normal vector") {
        Vec3 v = {1.0f, 0.0f, 0.0f};
        Vec3 normal = {0.0f, 0.0f, 0.0f};
        Vec3 result = vec3_reflect(&v, &normal);
        REQUIRE_THAT(result,
            Catch::Matchers::Predicate<Vec3>([](Vec3 v) { return vec3_is_nan(&v); }, "NaN")
        );
    }

    SECTION("Reflect a zero incoming vector") {
        Vec3 v = {0.0f, 0.0f, 0.0f};
        Vec3 normal = {1.0f, 0.0f, 0.0f};
        Vec3 result = vec3_reflect(&v, &normal);
        REQUIRE_THAT(result,
            Catch::Matchers::Predicate<Vec3>([](Vec3 v) { return vec3_is_nan(&v); }, "NaN")
        );
    }
}
