#include "phys_collision.h"

#include <math.h>

CollisionResult sphere_sphere(float r1, ac_vec3 *p1, float r2, ac_vec3 *p2) {
  CollisionResult ret;
  ret.collided = 0;

  ac_vec3 diffVec = ac_vec3_sub(p1, p2);

  float dist = ac_vec3_magnitude(&diffVec);

  ret.collided = (r1 + r2) > dist;

  if (ret.collided) {
    // contact normal
    ret.contactNormal = ac_vec3_sub(&p2, &p1);
    ret.contactNormal = ac_vec3_normalize(&ret.contactNormal);

    float rT = (r1 + r2);
    // pen depth
    ret.penetrationDepth = rT - dist;

    // contact point
    float ratio = ret.penetrationDepth / rT;

    // get contact of each sphere then set final as mid point between
    ac_vec3 contact1, contact2;

    contact1 = ac_vec3_scale(&ret.contactNormal, (r1 * ratio));
    contact2 = ac_vec3_scale(&ret.contactNormal, (r2 * ratio));

    ret.contactPoint = ac_vec3_add(&contact1, &contact2);
    ret.contactPoint =
        ac_vec3_scale(&ret.contactPoint, 0.5f); // get middle point with / 2
  }
  return ret;
}

CollisionResult
sphere_AABB(float r1, const ac_vec3 *const p1, ac_vec3 *aabbExtents,
            const ac_vec3 *const p2) { // MAKE COLLISION RESULT PROPERLY
  // https://stackoverflow.com/questions/28343716/sphere-intersection-test-of-aabb

  CollisionResult ret;
  ac_vec3 Bmin, Bmax;
  for (int i = 0; i < 3; i++) {
    Bmin.data[i] = p2->data[i] - aabbExtents->data[i];
    Bmax.data[i] = p2->data[i] + aabbExtents->data[i];
  }

  ac_vec3 closestPoint;
  for (int i = 0; i < 3; i++) {
    closestPoint.data[i] =
        fmaxf(Bmin.data[i], fminf(p1->data[i], Bmax.data[i]));
  }

  ac_vec3 diffVec = ac_vec3_sub(p1, &closestPoint);

  float distSquared = ac_vec3_dot(&diffVec, &diffVec);
  float r1Squared = r1 * r1;

  ret.collided = (distSquared <= r1Squared);
  if (ret.collided) {
    float dist = sqrtf(distSquared);
    // contact normal
    ret.contactNormal = ac_vec3_scale(&diffVec, 1.0f / dist);
    ret.penetrationDepth = r1 - dist;
    // contact point
    ret.contactPoint = ac_vec3_scale(&ret.contactNormal, ret.penetrationDepth);
    ret.contactPoint = ac_vec3_add(&ret.contactPoint, &closestPoint);
  }
  return ret;
}

CollisionResult
AABB_AABB(ac_vec3 *c1Extents, const ac_vec3 *const p1, ac_vec3 *c2Extents,
          const ac_vec3 *const p2) { // MAKE COLLISION RESULT PROPERLY

  // http://www.r-5.org/files/books/computers/algo-list/realtime-3d/Christer_Ericson-Real-Time_Collision_Detection-EN.pdf
  //  page 119
  // return !(
  //     abs(p1->x - p2->x) > (c1Extents->x + c2Extents->x) ||
  //     abs(p1->y - p2->y) > (c1Extents->y + c2Extents->y) ||
  //     abs(p1->z - p2->z) > (c1Extents->z + c2Extents->z));

  CollisionResult c;
  return c;
}

CollisionResult check_collision(Collider *c1, ac_vec3 *const p1, Collider *c2,
                                ac_vec3 *const p2) {
  // there is definitely a better way to do this
  if (c1->type == c2->type) {
    if (c1->type == SPHERE)
      return sphere_sphere(*(float *)c1->data, p1, *(float *)c2->data, p2);
    else
      return AABB_AABB((ac_vec3 *)c1->data, p1, (ac_vec3 *)c2->data, p2);
  }

  if (c1->type == SPHERE)
    return sphere_AABB(*(float *)c1->data, p1, (ac_vec3 *)c2->data, p2);
  else
    return sphere_AABB(*(float *)c2->data, p2, (ac_vec3 *)c1->data, p1);
}

void resolve_collision(CollisionResult *info, ac_vec3 *pos1, ac_vec3 *v1,
                       float m1, bool s1, ac_vec3 *pos2, ac_vec3 *v2, float m2,
                       bool s2) {

  ac_vec3 relativeVelocity = ac_vec3_sub(v2, v1);
  // vec3f_sub(&relativeVelocity, v2, v1); // Vr = v2 - v1

  float impulse = ac_vec3_dot(&relativeVelocity, &info->contactNormal);

  // calculate the impulse
  float impulseScalar =
      -(1.0f + 0.7f) * impulse; // 0.7 is the coefficient of restitution
  impulseScalar /= (s1 ? 0.0f : 1.0f / m1) + (s2 ? 0.0f : 1.0f / m2);

  // calc velocity changes
  ac_vec3 velocityChange1 =
      ac_vec3_scale(&info->contactNormal, impulseScalar / (s1 ? 1.0f : m1));
  ac_vec3 velocityChange2 =
      ac_vec3_scale(&info->contactNormal, impulseScalar / (s2 ? 1.0f : m2));

  // update if not static
  if (!s1) {
    *v1 = ac_vec3_sub(v1, &velocityChange1); // v1 -= velocityChange1
  }
  if (!s2) {
    *v2 = ac_vec3_add(v2, &velocityChange2); // v2 += velocityChange2
  }

  // DEPENETRATION
  const float penetrationSlop = 0.03f; // Adjust as needed
  float penetrationDepth =
      fmaxf(info->penetrationDepth - penetrationSlop, 0.0f);
  float totalInverseMass = (s1 ? 0.0f : 1.0f / m1) + (s2 ? 0.0f : 1.0f / m2);
  float depenetrationScalar =
      penetrationDepth / (totalInverseMass > 0.0f ? totalInverseMass : 1.0f);
  depenetrationScalar *= 10; // needed for aabbs because the force pushing them
                             // out wasn't great enough

  ac_vec3 correction = ac_vec3_scale(&info->contactNormal, depenetrationScalar);

  // apply position correction to non-static objects
  if (!s1) {
    *pos1 = ac_vec3_sub(pos1, &correction);
  }
  if (!s2) {
    *pos2 = ac_vec3_add(pos2, &correction);
  }
}