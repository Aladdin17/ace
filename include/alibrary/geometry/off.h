/**
 * \file
 * \author Christien Alden (34119981)
 * \brief OFF file format public functions and structures.
*/
#pragma once
#include <alibrary/math/vec3.h>
#include <stdint.h>
#include <stdio.h>

	/**
	 * \def MAX_LINE_LENGTH
	 * \brief The maximum length of a line in the OFF file format.
	*/
#define MAX_LINE_LENGTH 128
	/**
	 * \def MAX_LINE_TOKENS
	 * \brief The maximum number of tokens in a line in the OFF file format.
	*/
#define MAX_LINE_TOKENS 16

	/**
	 * \enum OFFFaceType
	 * \brief Represents the number of vertices in a face.
	*/
enum OFFFaceType
{
	UNDEFINED = 0,
	TRIANGLE = 3,
	QUAD = 4
};

	/**
	 * \struct OFFFace
	 * \brief Represents a triangle in the OFF file format.
	*/
struct OFFFace
{
	uint32_t* vertices;
	enum OFFFaceType type;
};

	/**
	 * \struct OFF
	 * \brief Represents an object in the OFF file format.
	*/
typedef struct OFF
{
	uint32_t numVertices;
	uint32_t numFaces;
	uint32_t numEdges;
	Vec3* vertices;
	struct OFFFace* faces;
} OFF;

	/**
	 * \brief Creates a new OFF object from the given stream.
	 * \param[in] stream The stream to read the OFF file from.
	 * \return A new OFF object, or NULL if the stream was invalid.
	 * \note If NULL is returned the \ref global_error_message will be set.
	*/
OFF* OFFImportFile(FILE* stream);
void OFFExportFile(OFF* obj, FILE* stream, int precision);
	/**
	 * \brief Destroys the given OFF object.
	 * \param[in,out] obj The object to destroy.
	 * \warning This function will set the given pointer to NULL.
	*/
void OFFDestroy(OFF** obj);
void OFFNormalise(OFF* obj, bool alignToOrigin);
