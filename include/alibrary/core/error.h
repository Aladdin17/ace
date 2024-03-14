/**
 * \file
 * \author Christien Alden
 * \brief A non-threadsafe dynamic error message system.
 * \details
 * Defines a global error message that can be set by the user from anywhere in the code.
 * The message is not threadsafe and should be used with caution.
*/
#pragma once
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * \def ERROR_MESSAGE_SIZE
	 * \brief The maximum size of the global error message.
	*/
#define ERROR_MESSAGE_SIZE 256

	/**
	 * \brief Set the global error message.
	 * \param[in] fmt printf style format string.
	 * \param[in] args va_list of arguments.
	 * \warning You must call va_end(args) after calling this function.
	 * \details
	 * This function allows you to set the global error message using a va_list of arguments.
	 * The size of the message after formatting is limited to \ref ERROR_MESSAGE_SIZE - 1
	 * characters, any characters beyond this limit are truncated.
	*/
void WriteGlobalErrorMessageVA(const char* fmt, va_list args);
	/**
	 * \brief Set the global error message.
	 * \param[in] fmt printf style format string.
	 * \param[in] ... printf style arguments.
	 * \details
	 * This function allows you to set the global error message using a printf style format string
	 * and arguments. The size of the message after formatting is limited to
	 * \ref ERROR_MESSAGE_SIZE - 1 characters, any characters beyond this limit are truncated.
	*/
void WriteGlobalErrorMessage(const char* fmt, ...);
	/**
	 * \brief Returns a const pointer to the error message.
	 * \return A const pointer to the error message.
	*/
const char* GetGlobalErrorMessage();

#ifdef __cplusplus
}
#endif
