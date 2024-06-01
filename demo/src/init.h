#pragma once

typedef struct pool_app pool_app;
typedef struct PhysWorld PhysWorld;
typedef struct pool_ball pool_ball;
typedef struct pool_table pool_table;
typedef struct cue_stick cue_stick;
typedef struct orbit_camera orbit_camera;
typedef struct frame_time frame_time;
typedef union ac_vec2 ac_vec2;
typedef union ac_vec3 ac_vec3;

void initialise_misc( pool_app* state );
void initialise_orbit_camera( orbit_camera* camera );
void initialise_frame_time( frame_time* time );
void initialise_physics_world( PhysWorld* world, int update_rate );
void initialise_pool_balls(PhysWorld *world, pool_ball **balls_ptr, int num_balls, int layout, void(*callback)(unsigned, unsigned), const ac_vec2* table_dimensions, const ac_vec3* table_center, const ac_vec2* cue_position, const ac_vec2* target_pos, const float drop_height);
void initialise_pool_table( PhysWorld* world, pool_table* table );
void initialise_cue_stick( cue_stick* stick );
ac_vec3 ball_start_pos_to_world_pos(
    const ac_vec2* start_position,
    const ac_vec3* surface_center,
    const ac_vec2* surface_dimensions,
    float          drop_height
);
