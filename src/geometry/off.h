/**
 * \file
 * \author Christien Alden
 * \brief OFF internal functions and structures.
 * \remark
 * This file contains internal declarations and should not be included directly;
 * include \ref alibrary/geometry/off.h instead. The file has been made available
 * for the purpose of documentation and testing.
*/
#pragma once
#include <core/config.h>
#include <alibrary/geometry/off.h>
#include <setjmp.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * \struct OFFReader
	 * \brief Contains the state of a parse of an OFF file.
	*/
typedef struct OFFReader
{
	jmp_buf errorContext;                ///< the error handling context
	FILE* stream;                        ///< the file stream to read from
	size_t lineNumber;                   ///< the current line number
	char line[OFF_MAX_LINE_LENGTH];      ///< holds the currently buffered line
	char* tokens[OFF_MAX_LINE_TOKENS];   ///< the tokens point to the line buffer
	size_t numTokens;                    ///< the number of tokens in the line
	bool hasHeader;                      ///< true if the file has an OFF descriptor
	OFF* obj;                            ///< the object being created
} OFFReader;


//--------------------------------------------------------------------------------------------------
// Internal Functions
//--------------------------------------------------------------------------------------------------

	/**
	 * \brief Removes any leading comments, empty lines, and OFF descriptor.
	 * \param[in] reader
	*/
INTERNAL void RemovePreamble(OFFReader* reader);
	/**
	 * \brief Initialises the OFF object
	 * \param[in] reader
	*/
INTERNAL void InitialiseOFF(OFFReader* reader);
	/**
	 * \brief Reads the counts of vertices, faces, and edges.
	 * \param[in] reader
	*/
INTERNAL void ReadCounts(OFFReader* reader);
	/**
	 * \brief Allocates memory for the vertices and faces.
	 * \param[in] reader
	*/
INTERNAL void AllocateVertices(OFFReader* reader);
	/**
	 * \brief Allocates memory for the faces.
	 * \param[in] reader
	*/
INTERNAL void AllocateFaces(OFFReader* reader);
	/**
	 * \brief Reads the vertices from the file.
	 * \param[in] reader
	*/
INTERNAL void ReadVertices(OFFReader* reader);
	/**
	 * \brief Reads the faces from the file.
	 * \param[in] reader
	*/
INTERNAL void ReadFaces(OFFReader* reader);
	/**
	 * \brief Deallocated the memory used by an face in an OFF object.
	 * \param[in] face
	*/
INTERNAL void DestroyOFFFace(OFFFace* face);


//--------------------------------------------------------------------------------------------------
// Utility Functions
//--------------------------------------------------------------------------------------------------

	/**
	 * \brief Handles an error by printing a message and longjmping to the error context.
	 * \param[in] buf the error context
	 * \param[in] fmt the format string
	 * \param[in] ... the format arguments
	*/
INTERNAL void HandleError(jmp_buf buf, const char* fmt, ...);
	/**
	 * \brief Seeks to the next candidate line in the file.
	 * \param[in] reader
	 * \retval true if a line was found
	 * \retval false if the end of the file was reached
	*/
INTERNAL bool SeekToNextLine(OFFReader* reader);
	/**
	 * \brief Determines whether a line is either a comment or empty line.
	 * \param[in] line the line to check
	 * \retval true if the line should be ignored
	 * \retval false if the line should be processed
	*/
INTERNAL bool ShouldIgnoreLine(const char* line);
	/**
	 * \brief Checks whether the end of the file is valid.
	 * \param[in] reader
	 * \details
	 * This function will throw an error if another candidate line is found;
	 * it is called after the file has been read to ensure that the file has
	 * been read correctly and does not contain any additional data (valid or not).
	 *
	 * \note
	 * Comments and empty lines do not trigged an error, as such you can add
	 * trailing comments to the file.
	*/
INTERNAL void ValidateFileTail(OFFReader* reader);

#ifdef __cplusplus
}
#endif
