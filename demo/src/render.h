#pragma once
#include <stdbool.h>

typedef struct pool_ball  pool_ball;
typedef union ac_vec3     ac_vec3;
typedef struct cue_stick  cue_stick;
typedef struct pool_app   pool_app;
typedef struct pool_table pool_table;
typedef struct PhysWorld  PhysWorld;

void draw_pool_ball(const pool_ball* ball, const ac_vec3* position);
void draw_cue_stick(const cue_stick* stick, const ac_vec3* position, float radius);
void draw_pool_table(const pool_table* table, bool orthographic);

void draw_scene(const pool_app* app, bool orthographic);
void draw_minimap(const pool_app* app);
void draw_powerbar(float power_ratio);
void draw_entity_info(const PhysWorld* world, unsigned entity_id);
