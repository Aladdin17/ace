#pragma once
#include "timer.h"

frame_time* app_init(void);
void        app_update_callback(int value);
void        app_key_callback(unsigned char key, int x, int y);
void        app_special_key_callback(int key, int x, int y);
void        app_render_callback(void);
