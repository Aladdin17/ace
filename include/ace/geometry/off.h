/**
 * \file
 * \author Christien Alden
 * \brief OFF functions and structures.
 */
#pragma once
#include <ace/math/vec3.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \def OFF_MAX_LINE_LENGTH
 * \brief The maximum length of a line in the OFF file format.
 */
#define OFF_MAX_LINE_LENGTH 128
/**
 * \def OFF_MAX_LINE_TOKENS
 * \brief The maximum number of tokens in a line in the OFF file format.
 */
#define OFF_MAX_LINE_TOKENS 16

/**
 * \enum OFFFaceType
 * \brief Represents the number of vertices in a face.
 */
typedef enum OFFFaceType
{
    OFFFaceType_UNDEFINED = 0,
    OFFFaceType_TRIANGLE  = 3,
    OFFFaceType_QUAD      = 4,
    OFFFaceType_MIN       = OFFFaceType_TRIANGLE,
    OFFFaceType_MAX       = OFFFaceType_QUAD
} OFFFaceType;

/**
 * \struct OFFFace
 * \brief Represents a face in an OFF object.
 */
typedef struct OFFFace
{
    uint32_t*   vertices;  ///< The indices of the vertices in the face.
    OFFFaceType type;      ///< Represents the type of the face (number of vertices).
} OFFFace;

/**
 * \struct OFF
 * \brief Represents an object in the OFF file format.
 */
typedef struct OFF
{
    uint32_t numVertices;  ///< The number of vertices in the object.
    uint32_t numFaces;     ///< The number of faces in the object.
    uint32_t numEdges;     ///< The number of edges in the object (not used).
    ac_vec3* vertices;     ///< The vertices in the object.
    OFFFace* faces;        ///< The faces in the object.
} OFF;

/**
 * \brief Creates a new OFF object from the given stream.
 * \param[in] stream The stream to read the OFF file from.
 * \return A new OFF object, or NULL if the stream was invalid.
 * \note If NULL is returned the global_error_message will be set, see \ref GetGlobalErrorMessage.
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

#ifdef __cplusplus
}
#endif
