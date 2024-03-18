/**
 * \file
 * \author Christien Alden
 * \brief String manipulation functions implementation.
 */
#include <alibrary/core/string.h>
#include <stdlib.h>
#include <string.h>

bool strToUint32(const char* str, uint32_t* value, int base)
{
    // check for NULL arguments
    if ( str == NULL || value == NULL )
    {
        return false;
    }

    // I am using strtoll instead of strtoul because of how the strtoul function
    // handles negative numbers (see https://en.cppreference.com/w/c/string/byte/strtoul).
    // Instead I allow strtoll to convert the string to a long long which provides enough precision
    // to cover the entire range of a uint32_t; which can then be cast to a uint32_t.
    char*     end;
    long long temp = strtoll(str, &end, base);

    // check that the value will fit in a uint32_t
    if ( temp < 0 || temp > UINT32_MAX )
    {
        return false;
    }

    // check that the entire string was used in the conversion
    if ( *end != '\0' )
    {
        return false;
    }

    *value = (uint32_t) temp;
    return true;
}

bool strToFloat(const char* str, float* value)
{
    // check for NULL arguments
    if ( str == NULL || value == NULL )
    {
        return false;
    }

    char* end;
    float temp = strtof(str, &end);

    // check that the entire string was used in the conversion
    if ( *end != '\0' )
    {
        return false;
    }

    *value = temp;
    return true;
}

size_t tokeniseString(char* str, char** tokens, size_t maxTokens, const char* delimiters)
{
    size_t count = 0;

#pragma warning(push)
#pragma warning(disable : 4996)  // disable msvc warning to use strtok_s instead of strtok

    // use strtok to split the str into tokens
    char* token = strtok(str, delimiters);
    while ( token != NULL && count < maxTokens )
    {
        // store the token in the array and increment the count
        tokens[count++] = token;

        // get the next token
        token = strtok(NULL, delimiters);
    }

#pragma warning(pop)

    return count;
}

void removeNewlineChar(char* str)
{
    // check first for windows style newline
    char* newline = strchr(str, '\r');
    if ( newline == NULL )
    {
        // if not found, check for unix style newline
        newline = strchr(str, '\n');
    }

    // if a newline character was found, replace it with a null terminator
    if ( newline != NULL )
    {
        *newline = '\0';
    }
}
