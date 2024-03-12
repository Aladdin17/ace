/**
 * \file
 * \brief A non-threadsafe dynamic error message.
 * \details
 * Defines a global error message that can be set by the user from anywhere in the code.
 * The message is not threadsafe and should be used with caution.
*/
#pragma once
#include <stdarg.h>

#define ERROR_MESSAGE_SIZE 256

	/**
	 * \brief Set the global error message.
	 * \param[in] fmt printf style format string.
	 * \param[in] args va_list of arguments.
	 * \note The message is truncated to ERROR_MESSAGE_SIZE - 1 characters.
	 * \note You must call va_end(args) after calling this function.
	*/
void WriteGlobalErrorMessageVA(const char* fmt, va_list args);
	/**
	 * \brief Set the global error message.
	 * \param[in] fmt printf style format string.
	 * \param[in] ... printf style arguments.
	 * \note
	 * The message is truncated to ERROR_MESSAGE_SIZE - 1 characters.
	*/
void WriteGlobalErrorMessage(const char* fmt, ...);
	/**
	 * \brief Get the global error message.
	 * \return The global error message.
	*/
const char* GetGlobalErrorMessage();
