/**
 * \file
 * \author Christien Alden
 * \brief Defines the frame_time struct that is needed by both the app and opengl initialisation.
 */
#pragma once

typedef struct frame_time
{
    int update_rate;
    int last_frame_time;
    int current_frame_time;
} frame_time;
