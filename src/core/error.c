/**
 * \file
 * \author Christien Alden
 * \brief Error message system implementation.
*/
#include <ALibrary/core/error.h>
#include <stdio.h>

static char global_error_message[ERROR_MESSAGE_SIZE] = { '\0' };

void WriteGlobalErrorMessageVA(const char* fmt, va_list args)
{
	vsnprintf(global_error_message, ERROR_MESSAGE_SIZE, fmt, args);
	global_error_message[ERROR_MESSAGE_SIZE - 1] = '\0';
}

void WriteGlobalErrorMessage(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	WriteGlobalErrorMessageVA(fmt, args);
	va_end(args);
}

const char* GetGlobalErrorMessage()
{
	return global_error_message;
}
