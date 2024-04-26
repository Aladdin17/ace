#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_assertion_result.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/matchers/catch_matchers_predicate.hpp>
#include <alibrary/math/vec3.h>

TEST_CASE("vec3 operations", "[vec3]") {
    vec3 a = {1.0f, 2.0f, 3.0f};
    vec3 b = {4.0f, 5.0f, 6.0f};
    vec3 nan = vec3_nan();
    vec3 zero = vec3_zero();
    vec3 result;

    SECTION("vec3_zero") {
        REQUIRE_THAT(zero.x,
            Catch::Matchers::WithinRel(0.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(0.0f, AL_EPSILON));
        REQUIRE_THAT(zero.y,
            Catch::Matchers::WithinRel(0.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(0.0f, AL_EPSILON));
        REQUIRE_THAT(zero.z,
            Catch::Matchers::WithinRel(0.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(0.0f, AL_EPSILON));
    }

    SECTION("vec3_nan") {
        REQUIRE_THAT(nan.x,
            Catch::Matchers::Predicate<float>([](float x) { return isnan(x); }, "NaN")
        );
        REQUIRE_THAT(nan.y,
            Catch::Matchers::Predicate<float>([](float x) { return isnan(x); }, "NaN")
        );
        REQUIRE_THAT(nan.z,
            Catch::Matchers::Predicate<float>([](float x) { return isnan(x); }, "NaN")
        );
    }

    SECTION("vec3_is_zero") {
        vec3 non_zero = { 0.0f, 0.0f, 1.0f };
        vec3 negative_non_zero = { 0.0f, 0.0f, -1.0f };
        REQUIRE(vec3_is_zero(&zero) == true);
        REQUIRE(vec3_is_zero(&non_zero) == false);
        REQUIRE(vec3_is_zero(&negative_non_zero) == false);
    }

    SECTION("vec3_is_nan") {
        vec3 nan_1 = {NAN, 0.0f, 0.0f};
        vec3 nan_2 = {0.0f, NAN, 0.0f};
        vec3 nan_3 = {0.0f, 0.0f, NAN};
        REQUIRE(vec3_is_nan(&nan) == true);
        REQUIRE(vec3_is_nan(&nan_1) == true);
        REQUIRE(vec3_is_nan(&nan_2) == true);
        REQUIRE(vec3_is_nan(&nan_3) == true);
        REQUIRE(vec3_is_nan(&zero) == false);
    }

    SECTION("vec3_add") {
        result = vec3_add(&a, &b);
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
    }

    SECTION("vec3_sub") {
        result = vec3_sub(&a, &b);
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
    }

    SECTION("vec3_negate") {
        result = vec3_negate(&a);
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
    }

    SECTION("vec3_scale") {
        result = vec3_scale(&a, 2.0f);
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
    }

    SECTION("vec3_dot") {
        float dot;
        vec3 first = { 1.0f, 0.0f, 0.0f };
        vec3 second = { 0.0f, 1.0f, 0.0f };
        vec3 third = { -1.0f, 0.0f, 0.0f };

        // Dot product of perpendicular vectors is 0
        dot = vec3_dot(&first, &second);
        REQUIRE_THAT(dot,
            Catch::Matchers::WithinRel(0.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(0.0f, AL_EPSILON)
        );

        // Dot product of vectors facing the opposite direction is < 0
        dot = vec3_dot(&first, &third);
        REQUIRE(dot < 0.0f);

        // Dot product of vectors facing the same direction is > 0
        dot = vec3_dot(&first, &first);
        REQUIRE( dot > 0.0f );
    }

    SECTION("vec3_cross") {
        result = vec3_cross(&a, &b);
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
    }

    SECTION("vec3_magnitude") {
        float mag = vec3_magnitude(&a);
        REQUIRE_THAT(mag,
            Catch::Matchers::WithinRel(3.741657f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(3.741657f, AL_EPSILON)
        );
    }

    SECTION("vec3_normalize") {
        // guard nan vectors
        result = vec3_normalize(&nan);
        REQUIRE_THAT(result,
            Catch::Matchers::Predicate<vec3>([](vec3 v) { return vec3_is_nan(&v); }, "NaN")
        );

        // guard zero vectors
        result = vec3_normalize(&zero);
        REQUIRE_THAT(result,
            Catch::Matchers::Predicate<vec3>([](vec3 v) { return vec3_is_nan(&v); }, "NaN")
        );

        // check valid vector
        result = vec3_normalize(&a);
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

TEST_CASE("vec3 utilities angle", "[vec3]") {
    SECTION("Zero magnitude vectors") {
        vec3 a = {0.0f, 0.0f, 0.0f};
        vec3 b = {1.0f, 0.0f, 0.0f};
        float angle = al_rad_to_deg(vec3_angle(&a, &b));
        REQUIRE_THAT(angle,
            Catch::Matchers::Predicate<float>([](float x) { return isnan(x); }, "NaN")
        );

        angle = al_rad_to_deg(vec3_angle(&b, &a));
        REQUIRE_THAT(angle,
            Catch::Matchers::Predicate<float>([](float x) { return isnan(x); }, "NaN")
        );
    }

    SECTION("NaN vectors") {
        vec3 a = {NAN, 0.0f, 0.0f};
        vec3 b = {1.0f, 0.0f, 0.0f};
        float angle = al_rad_to_deg(vec3_angle(&a, &b));
        REQUIRE_THAT(angle,
            Catch::Matchers::Predicate<float>([](float x) { return isnan(x); }, "NaN")
        );

        angle = al_rad_to_deg(vec3_angle(&b, &a));
        REQUIRE_THAT(angle,
            Catch::Matchers::Predicate<float>([](float x) { return isnan(x); }, "NaN")
        );
    }

    SECTION("90 degrees angle") {
        vec3 a = {1.0f, 0.0f, 0.0f};
        vec3 b = {0.0f, 1.0f, 0.0f};
        float result = al_rad_to_deg(vec3_angle(&a, &b));
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(90.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(90.0f, AL_EPSILON)
        );
    }

    SECTION("45 degrees angle") {
        vec3 a = {1.0f, 1.0f, 0.0f};
        vec3 b = {1.0f, 0.0f, 0.0f};
        float result = al_rad_to_deg(vec3_angle(&a, &b));
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(45.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(45.0f, AL_EPSILON)
        );
    }

    SECTION("0 degrees angle") {
        vec3 a = {1.0f, 0.0f, 0.0f};
        vec3 b = {1.0f, 0.0f, 0.0f};
        float result = al_rad_to_deg(vec3_angle(&a, &b));
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(0.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(0.0f, AL_EPSILON)
        );
    }

    SECTION("'-90' degrees angle") {
        vec3 a = {1.0f, 0.0f, 0.0f};
        vec3 b = {0.0f, -1.0f, 0.0f};
        float result = al_rad_to_deg(vec3_angle(&a, &b));
        REQUIRE_THAT(result,
            Catch::Matchers::WithinRel(90.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(90.0f, AL_EPSILON)
        );
    }
}

TEST_CASE("vec3 utilities reflect", "[vec3]") {
    SECTION("NaN normal vector") {
        vec3 n = vec3_nan();
        vec3 v = {1.0f, 0.0f, 0.0f};
        vec3 result = vec3_reflect(&v, &n);
        REQUIRE_THAT(result,
            Catch::Matchers::Predicate<vec3>([](vec3 v) { return vec3_is_nan(&v); }, "NaN")
        );
    }

    SECTION("NaN incoming vector") {
        vec3 n = {1.0f, 0.0f, 0.0f};
        vec3 v = vec3_nan();
        vec3 result = vec3_reflect(&v, &n);
        REQUIRE_THAT(result,
            Catch::Matchers::Predicate<vec3>([](vec3 v) { return vec3_is_nan(&v); }, "NaN")
        );
    }

    SECTION("Reflect a zero normal vector") {
        vec3 v = {1.0f, 0.0f, 0.0f};
        vec3 n = {0.0f, 0.0f, 0.0f};
        vec3 result = vec3_reflect(&v, &n);
        REQUIRE_THAT(result,
            Catch::Matchers::Predicate<vec3>([](vec3 v) { return vec3_is_nan(&v); }, "NaN")
        );
    }

    SECTION("Reflect a zero incoming vector") {
        vec3 v = {0.0f, 0.0f, 0.0f};
        vec3 n = {1.0f, 0.0f, 0.0f};
        vec3 result = vec3_reflect(&v, &n);
        REQUIRE_THAT(result,
            Catch::Matchers::Predicate<vec3>([](vec3 v) { return vec3_is_nan(&v); }, "NaN")
        );
    }

    SECTION("Reflect a perpendicular vector") {
        vec3 v = {1.0f, 0.0f, 0.0f};
        vec3 n = {0.0f, 1.0f, 0.0f};
        vec3 result = vec3_reflect(&v, &n);
        REQUIRE_THAT(result.x,
            Catch::Matchers::WithinRel(1.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(1.0f, AL_EPSILON)
        );
        REQUIRE_THAT(result.y,
            Catch::Matchers::WithinRel(0.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(0.0f, AL_EPSILON)
        );
        REQUIRE_THAT(result.z,
            Catch::Matchers::WithinRel(0.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(0.0f, AL_EPSILON)
        );
    }

    SECTION("Reflect a parallel vector") {
        vec3 v = {1.0f, 0.0f, 0.0f};
        vec3 n = {1.0f, 0.0f, 0.0f};
        vec3 result = vec3_reflect(&v, &n);
        REQUIRE_THAT(result.x,
            Catch::Matchers::WithinRel(-1.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(-1.0f, AL_EPSILON)
        );
        REQUIRE_THAT(result.y,
            Catch::Matchers::WithinRel(0.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(0.0f, AL_EPSILON)
        );
        REQUIRE_THAT(result.z,
            Catch::Matchers::WithinRel(0.0f, AL_EPSILON) ||
            Catch::Matchers::WithinAbs(0.0f, AL_EPSILON)
        );
    }

    SECTION("Reflecting a vector") {
        vec3 v = {1.0f, 1.0f, 0.0f};
        vec3 n = {0.0f, 1.0f, 0.0f};
        vec3 result = vec3_reflect(&v, &n);
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
}
