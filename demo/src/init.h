#pragma once

typedef struct PhysWorld PhysWorld;
typedef struct pool_ball pool_ball;
typedef struct pool_table pool_table;
typedef struct cue_stick cue_stick;
typedef struct orbit_camera orbit_camera;

void initialise_orbit_camera( orbit_camera* camera );
void initialise_physics_world( PhysWorld* world, int update_rate );
void initialise_pool_balls( PhysWorld* world, pool_ball* balls, int* num_balls );
void initialise_pool_table( PhysWorld* world, pool_table* table );
void initialise_cue_stick( cue_stick* stick );
