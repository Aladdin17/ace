#pragma once

typedef struct pool_ball pool_ball;
typedef union ac_vec3 ac_vec3;
typedef struct cue_stick cue_stick;
typedef struct pool_app pool_app;

void draw_pool_ball( const pool_ball* ball, const ac_vec3* position );
void draw_cue_stick( const cue_stick* stick, const ac_vec3* position, float radius );

void draw_scene( const pool_app* app );
void draw_minimap( const pool_app* app );
void draw_powerbar( float power_ratio );
