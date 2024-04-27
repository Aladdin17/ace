#include <alibrary/math/math.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

TEST_CASE( "al_deg_to_rad", "math" ) {
    auto [input, expected] = GENERATE( Catch::Generators::table<float, float>({
        { 0.0f, 0.0f },
        { 90.0f, 1.5707963267948966f },
        { 180.0f, 3.141592653589793f },
        { 270.0f, 4.71238898038469f },
        { 360.0f, 6.283185307179586f },
    }));

    CAPTURE(input, expected);
    REQUIRE_THAT( al_deg_to_rad(input),
        Catch::Matchers::WithinRel(expected, AL_EPSILON)
    );
}

TEST_CASE( "al_rad_to_deg", "[math]" ) {
    auto [input, expected] = GENERATE( Catch::Generators::table<float, float>({
        { 0.0f, 0.0f },
        { 1.5707963267948966f, 90.0f },
        { 3.141592653589793f, 180.0f },
        { 4.71238898038469f, 270.0f },
        { 6.283185307179586f, 360.0f },
    }));

    CAPTURE(input, expected);
    REQUIRE_THAT( al_rad_to_deg(input),
        Catch::Matchers::WithinRel(expected, AL_EPSILON)
    );
}

TEST_CASE( "al_clamp", "[math]" ) {
    auto [value, min, max, expected] = GENERATE( Catch::Generators::table<float, float, float, float>({
        { -1.0f, 0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f, 0.0f },
        { 0.5f, 0.0f, 1.0f, 0.5f },
        { 1.0f, 0.0f, 1.0f, 1.0f },
        { 1.5f, 0.0f, 1.0f, 1.0f },
    }));

    CAPTURE(value, min, max, expected);
    REQUIRE_THAT( al_clamp(value, min, max),
        Catch::Matchers::WithinRel(expected, AL_EPSILON)
    );
}
