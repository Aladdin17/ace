

#include <stdio.h>

#include <ace/math/vec3.h>
#include <ace/physics/phys_world.h>
#include <ace/render/camera.h>

PhysWorld physicsWorld;

unsigned cueBallID;
Collider cueBallCollider;

unsigned ballIDs[10];
ac_vec3 ballPositions[10];
Collider ballColliders[10];

ac_vec3 floorAABBPos = {0, 0, 0};
Collider floorCol;


void init(void);

int main(void)
{
    printf("Yessir");
}

void init(void)
{
    
}