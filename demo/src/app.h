/**
 * \file
 * \author Christien Alden
 * \brief Application callbacks and initialisation for demo.
 */
#pragma once
#include "timer.h"

/**
 * \brief Initialises the application.
 * \return A pointer to the frame_time struct.
 */
frame_time* app_init(void);
/**
 * \brief Updates the application.
 * \param[in] value The value passed to the callback.
 */
void        app_update_callback(int value);
/**
 * \brief Handles key presses.
 * \param[in] key The key pressed.
 * \param[in] x The x position of the mouse.
 * \param[in] y The y position of the mouse.
 */
void        app_key_callback(unsigned char key, int x, int y);
/**
 * \brief Handles special key presses.
 * \param[in] key The key pressed.
 * \param[in] x The x position of the mouse.
 * \param[in] y The y position of the mouse.
 */
void        app_special_key_callback(int key, int x, int y);
/**
 * \brief Renders the application.
 */
void        app_render_callback(void);
