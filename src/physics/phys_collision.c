#include "ace/physics/phys_collision.h"
#include <math.h>
typedef IntersectionResult (*collision_detection_func)(
    const Collider* c1, const ac_vec3* p1, const Collider* c2, const ac_vec3* p2
);

static const collision_detection_func collisionDetectionFunctions[2][2] = {
    // 3hrs of my life was spent on finding that this indexing was wrong
    // SPHERE_C, AABB_C
    { sphere_sphere, sphere_AABB }, // SPHERE_C
    {   AABB_sphere,        NULL }  // AABB_C
};

IntersectionResult check_collision(Collider* c1, ac_vec3* const p1, Collider* c2, ac_vec3* const p2)
{
    collision_detection_func func = collisionDetectionFunctions[c1->type][c2->type];
    if ( func == NULL )
    {
        // no collision detection function exists
        return (IntersectionResult){ .intersected      = false,
                                     .contactNormal    = ac_vec3_nan(),
                                     .penetrationDepth = NAN,
                                     .contactPoint     = ac_vec3_nan() };
    }

    return func(c1, p1, c2, p2);
}

void resolve_collision(
    IntersectionResult* info,
    ac_vec3*            pos1,
    ac_vec3*            v1,
    float               m1,
    bool                s1,
    ac_vec3*            pos2,
    ac_vec3*            v2,
    float               m2,
    bool                s2
)
{
    if ( ac_vec3_is_nan(&info->contactNormal) || ac_vec3_is_nan(&info->contactPoint) )
    {
        return;
    }

    ac_vec3 relativeVelocity = ac_vec3_sub(v2, v1);

    float impulse = ac_vec3_dot(&relativeVelocity, &info->contactNormal);
    // if objects are moving towards eachother or either entity is static
    if ( impulse <= 0.0f || (s1 || s2) )
    {
        // calculate the impulse
        float impulseScalar  = -(1.0f + 0.8f) * impulse;  // 0.8 is the coefficient of restitution
        impulseScalar       /= (s1 ? 0.0f : 1.0f / m1) + (s2 ? 0.0f : 1.0f / m2);

        // calc velocity changes
        ac_vec3 velocityChange1, velocityChange2;
        velocityChange1 = ac_vec3_scale(&info->contactNormal, impulseScalar / (s1 ? 1.0f : m1));
        velocityChange2 = ac_vec3_scale(&info->contactNormal, impulseScalar / (s2 ? 1.0f : m2));

        const float penetrationSlop  = 0.001f;  // max allowed overlap before depenetration
        float       penetrationDepth = fmaxf(info->penetrationDepth - penetrationSlop, 0.0f);

        // depenetration is variable on the speed. higher speed = bigger depen
        float relativeSpeed = ac_vec3_magnitude(&relativeVelocity);

        float totalInverseMass = (s1 ? 0.0f : 1.0f / m1) + (s2 ? 0.0f : 1.0f / m2);
        float depenetrationScalar =
            penetrationDepth / (totalInverseMass > 0.0f ? totalInverseMass : 1.0f);

        // scale depen with relative speed
        depenetrationScalar *= fmaxf(1.0f, relativeSpeed * 0.1f);

        ac_vec3 correction = ac_vec3_scale(&info->contactNormal, depenetrationScalar);

        // apply velocity and position correction to non-static objects
        if ( !s1 )
        {
            *v1   = ac_vec3_sub(v1, &velocityChange1);  // v1 -= velocityChange1
            *pos1 = ac_vec3_sub(pos1, &correction);
        }

        if ( !s2 )
        {
            *v2   = ac_vec3_add(v2, &velocityChange2);  // v2 += velocityChange2
            *pos2 = ac_vec3_add(pos2, &correction);
        }
    }
}

// ac_vec3 relativeVelocity = ac_vec3_sub(v2, v1);

// float impulse = ac_vec3_dot(&relativeVelocity, &info->contactNormal);
// // if objects are moving towards eachother or either entity is static
// if ( impulse <= 0.0f || (s1 || s2) )
// {
//     // momentums before collision
//     float momentum_body_1 = m1 * ac_vec3_magnitude(v1);
//     float momentum_body_2 = m2 * ac_vec3_magnitude(v2);
//     float total_momentum = momentum_body_1 + momentum_body_2;
//     const float restitution = 0.8f;
//     float total_momentum_after_collision = total_momentum * restitution;

//     // calc velocity changes
//     ac_vec3 body_1_reflection = ac_vec3_reflect(v1, &info->contactNormal);
//     body_1_reflection = ac_vec3_normalize(&body_1_reflection);
//     ac_vec3 body_2_reflection = ac_vec3_reflect(v2, &info->contactNormal);
//     body_2_reflection = ac_vec3_normalize(&body_2_reflection);

//     const float penetrationSlop  = 0.001f;  // max allowed overlap before depenetration
//     float       penetrationDepth = fmaxf(info->penetrationDepth - penetrationSlop, 0.0f);

//     // depenetration is variable on the speed. higher speed = bigger depen
//     float relativeSpeed = ac_vec3_magnitude(&relativeVelocity);

//     float totalInverseMass = (s1 ? 0.0f : 1.0f / m1) + (s2 ? 0.0f : 1.0f / m2);
//     float depenetrationScalar =
//         penetrationDepth / (totalInverseMass > 0.0f ? totalInverseMass : 1.0f);

//     // scale depen with relative speed
//     depenetrationScalar *= fmaxf(1.0f, relativeSpeed * 0.1f);

//     ac_vec3 correction = ac_vec3_scale(&info->contactNormal, depenetrationScalar);

//     // apply velocity and position correction to non-static objects
//     if ( !s1 )
//     {
//         float post_momentum_1 = momentum_body_1 / total_momentum *
//         total_momentum_after_collision; float velocity_1_scalar = post_momentum_1 / m1; *v1   =
//         ac_vec3_scale(&body_1_reflection, velocity_1_scalar); *pos1 = ac_vec3_sub(pos1,
//         &correction);
//     }

//     if ( !s2 )
//     {
//         float post_momentum_2 = momentum_body_2 / total_momentum *
//         total_momentum_after_collision; float velocity_2_scalar = post_momentum_2 / m2; *v2   =
//         ac_vec3_scale(&body_2_reflection, velocity_2_scalar); *pos2 = ac_vec3_add(pos2,
//         &correction);
//     }

// if ( ac_vec3_is_nan(&info->contactNormal) || ac_vec3_is_nan(&info->contactPoint) )
// {
//     return;
// }

// ac_vec3 relativeVelocity = ac_vec3_sub(v2, v1);

// float impulse = ac_vec3_dot(&relativeVelocity, &info->contactNormal);
// // if objects are moving towards eachother or either entity is static
// if ( impulse <= 0.0f || (s1 || s2) )
// {
//     // calculate the impulse
//     float total_mass = m1 + m2;
//     if ( total_mass >= 0.0f )
//     {
//         return;
//     }

//     // calaculate impulse scalar
//     float total_inverse_mass = 1.0f / total_mass;
//     float lambda  = -(1.0f + 0.8f) * impulse / total_inverse_mass;  // 0.8 is the coefficient of
//     restitution ac_vec3 linear_impulse = ac_vec3_scale(&info->contactNormal, lambda);

//     // depenetrate
//     const float penetrationSlop  = 0.001f;  // max allowed overlap before depenetration
//     float       penetrationDepth = fmaxf(info->penetrationDepth - penetrationSlop, 0.0f);
//     float depenetrationScalar = info->penetrationDepth * total_inverse_mass;

//     // depenetration is variable on the speed. higher speed = bigger depen
//     float relativeSpeed = ac_vec3_magnitude(&relativeVelocity);
//     depenetrationScalar *= fmaxf(1.0f, relativeSpeed * 0.1f);
//     ac_vec3 correction = ac_vec3_scale(&info->contactNormal, depenetrationScalar);

//     // calc velocity changes
//     ac_vec3 velocityChange1, velocityChange2;
//     velocityChange1 = ac_vec3_scale(&info->contactNormal, lambda / m1);
//     velocityChange2 = ac_vec3_scale(&info->contactNormal, lambda / m2);

//     // apply velocity and position correction to non-static objects
//     if ( !s1 )
//     {
//         *v1   = ac_vec3_add(v1, &velocityChange1);  // v1 -= velocityChange1
//         *pos1 = ac_vec3_add(pos1, &correction);
//     }

//     if ( !s2 )
//     {
//         *v2   = ac_vec3_sub(v2, &velocityChange2);  // v2 += velocityChange2
//         *pos2 = ac_vec3_sub(pos2, &correction);
//     }
// }
