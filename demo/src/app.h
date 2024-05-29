#pragma once
#include "timer.h"

void app_update_callback( int value );
frame_time* app_init( void );
void app_key_callback( unsigned char key, int x, int y );
void app_special_key_callback( int key, int x, int y );
void app_render_callback( void );
