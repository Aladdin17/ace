#include <ace/geometry/intersection.h>
#include <math.h>
#

IntersectionResult sphere_sphere(
    const Collider* c1, const ac_vec3* p1, const Collider* c2, const ac_vec3* p2
)
{
    Sphere* sphere1 = (Sphere*) c1->data;
    Sphere* sphere2 = (Sphere*) c2->data;

    IntersectionResult ret;
    ret.intersected = 0;

    ac_vec3 diffVec = ac_vec3_sub(p1, p2);

    float dist = ac_vec3_magnitude(&diffVec);

    ret.intersected = (sphere1->radius + sphere2->radius) > dist;

    if ( ret.intersected )
    {
        // contact normal
        ret.contactNormal = ac_vec3_sub(p2, p1);
        ret.contactNormal = ac_vec3_normalize(&ret.contactNormal);

        float rT             = (sphere1->radius + sphere2->radius);
        // pen depth
        ret.penetrationDepth = rT - dist;

        // contact point
        float ratio = ret.penetrationDepth / rT;

        // get contact of each sphere then set final as mid point between
        ac_vec3 contact1, contact2;

        contact1 = ac_vec3_scale(&ret.contactNormal, (sphere1->radius * ratio));
        contact2 = ac_vec3_scale(&ret.contactNormal, (sphere2->radius * ratio));

        ret.contactPoint = ac_vec3_add(&contact1, &contact2);
        ret.contactPoint = ac_vec3_scale(&ret.contactPoint, 0.5f);  // get middle point with / 2
    }
    return ret;
}

IntersectionResult sphere_AABB(
    const Collider* c1, const ac_vec3* p1, const Collider* c2, const ac_vec3* p2
)
{
    Sphere* sphere          = (Sphere*) c1->data;
    AABB*   aabbHalfExtents = (AABB*) c2->data;

    // https://stackoverflow.com/questions/28343716/sphere-intersection-test-of-aabb

    IntersectionResult ret;
    ac_vec3            Bmin, Bmax;
    for ( int i = 0; i < 3; i++ )
    {
        Bmin.data[i] = p2->data[i] - aabbHalfExtents->half_extents.data[i];
        Bmax.data[i] = p2->data[i] + aabbHalfExtents->half_extents.data[i];
    }

    ac_vec3 closestPoint;
    for ( int i = 0; i < 3; i++ )
    {
        closestPoint.data[i] = fmaxf(Bmin.data[i], fminf(p1->data[i], Bmax.data[i]));
    }

    ac_vec3 diffVec = ac_vec3_sub(p1, &closestPoint);

    float distSquared         = ac_vec3_dot(&diffVec, &diffVec);
    float sphereRadiusSquared = sphere->radius * sphere->radius;

    ret.intersected = (distSquared <= sphereRadiusSquared);
    if ( ret.intersected )
    {
        float dist           = sqrtf(distSquared);
        // contact normal
        ret.contactNormal    = ac_vec3_scale(&diffVec, 1.0f / dist);
        ret.penetrationDepth = sphere->radius - dist;
        // contact point
        ret.contactPoint     = ac_vec3_scale(&ret.contactNormal, ret.penetrationDepth);
        ret.contactPoint     = ac_vec3_add(&ret.contactPoint, &closestPoint);
    }
    return ret;
}

IntersectionResult AABB_sphere(
    const Collider* c1, const ac_vec3* p1, const Collider* c2, const ac_vec3* p2
)
{
    return sphere_AABB(c2, p2, c1, p1);
}

// IntersectionResult // we dont need to implement
// AABB_AABB(ac_vec3 *c1Extents, const ac_vec3 *const p1, ac_vec3 *c2Extents,
//           const ac_vec3 *const p2) { // MAKE COLLISION RESULT PROPERLY

//   //
//   http://www.r-5.org/files/books/computers/algo-list/realtime-3d/Christer_Ericson-Real-Time_Collision_Detection-EN.pdf
//   //  page 119
//   // return !(
//   //     abs(p1->x - p2->x) > (c1Extents->x + c2Extents->x) ||
//   //     abs(p1->y - p2->y) > (c1Extents->y + c2Extents->y) ||
//   //     abs(p1->z - p2->z) > (c1Extents->z + c2Extents->z));

//   IntersectionResult c;
//   return c;
// }
