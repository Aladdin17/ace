/**
 * \file
 * \author Christien Alden
 * \brief String manipulation functions.
 */
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
 * \details
 * This function converts the given string to an unsigned 32-bit integer. The \p base parameter
 * specifies the base of the number in the string. If the string is not a positive integer or the
 * number is too large to fit in a uint32_t then the function will return false. If the string
 * was successfully converted then the value of \p value will be updated with the converted value.
 *
 * \note
 * Unlike the standard library function strtoul, this function will return false if the string
 * contains a negative number. This is because the strtoul function will convert a negative number
 * to a positive number and return true. This function will return false if the string contains a
 * negative number.
 */
bool   strToUint32(const char* str, uint32_t* value, int base);
/**
 * \brief Attempts to convert a string to a float.
 * \param[in] str The string to convert.
 * \param[out] value The converted value.
 * \retval true the string was converted
 * \retval false the string was not a valid float
 * \note If the string was not a valid float the value of 'value' will be unchanged.
 */
bool   strToFloat(const char* str, float* value);
/**
 * \brief Tokenises a string into an array of tokens.
 * \param[in] str The string to tokenise.
 * \param[out] tokens The array of tokens.
 * \param[in] maxTokens The maximum number of tokens to store.
 * \param[in] delimiters The delimiters to use when tokenising the string.
 * \return The number of tokens found.
 * \warning Failure to make \p tokens large enough will result in undefined behaviour.
 * \details
 * This function tokenises a string into an array of tokens. The tokens are separated by the
 * delimiters specified in the \p delimiters string. The tokens are stored in the \p tokens array
 * and the number of tokens found is returned. The \p maxTokens parameter specifies the maximum
 * number of tokens to store in the \p tokens array. If the \p tokens array is not large enough to
 * store all of the tokens then the behaviour is undefined. The \p delimiters string is a C-string
 * containing the characters to use for delimiting the tokens; e.g., " \t" will use space and tab
 * characters as delimiters.
 */
size_t tokeniseString(char* str, char** tokens, size_t maxTokens, const char* delimiters);
/**
 * \brief Removes the newline character from the end of a string.
 * \param[in,out] str The string to remove the newline character from.
 * \details
 * This function removes the newline character from the end of a string.
 * The function will detect and remove both Unix and Windows style newline characters.
 */
void   removeNewlineChar(char* str);

#ifdef __cplusplus
}
#endif
