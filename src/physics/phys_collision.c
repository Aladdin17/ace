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
