/**
 * \file
 * \author Christien Alden
 * \brief Error message system implementation.
 */
#include <alibrary/core/error.h>
#include <stdio.h>

static char global_error_message[ERROR_MESSAGE_SIZE] = { '\0' };

void WriteGlobalErrorMessageVA(const char* fmt, va_list args)
{
    if (fmt == NULL)
    {
        global_error_message[0] = '\0';
        return;
    }

#ifdef __STD_LIB_EXT1__
    vsnprintf_s(global_error_message, sizeof(global_error_message), _TRUNCATE, fmt, args);
#else
    // NOLINTNEXTLINE(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
    vsnprintf(global_error_message, sizeof(global_error_message), fmt, args);
    global_error_message[sizeof(global_error_message) - 1] = '\0';
#endif
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
