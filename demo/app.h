#pragma once

typedef struct
{
    int update_rate;
    int last_frame_time;
    int current_frame_time;
} frame_time;

void app_update( int value );
frame_time* app_init( void );
void app_key_callback( unsigned char key, int x, int y );
void app_special_key_callback( int key, int x, int y );
void app_render_callback( void );
