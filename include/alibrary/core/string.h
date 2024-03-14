#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * \brief Converts the given string to an unsigned 32-bit integer.
	 * \param[in] str The string to convert.
	 * \param[out] value The converted value.
	 * \param[in] base The base of the number in the string.
	 * \retval true the string was converted
	 * \retval false the string was not a valid uint32
	 * \note If the string was not a valid uint32 the value of 'value' will be unchanged.
	*/
bool strToUint32(const char* str, uint32_t* value, int base);
bool strToFloat(const char* str, float* value);
size_t tokeniseLine(char* line, char** tokens, size_t maxTokens);
void removeNewlineChar(char* str);

#ifdef __cplusplus
}
#endif
