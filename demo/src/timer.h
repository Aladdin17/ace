/**
 * \file
 * \author Christien Alden
 * \brief Defines the frame_time struct that is needed by both the app and opengl initialisation.
 */
#pragma once

/**
 * \struct frame_time
 * \brief Structure to hold the data for frame timing.
 */
typedef struct frame_time
{
    int update_rate;         ///< The update rate of the application, in Hz.
    int last_frame_time;     ///< The time of the last frame.
    int current_frame_time;  ///< The time of the current frame.
} frame_time;
