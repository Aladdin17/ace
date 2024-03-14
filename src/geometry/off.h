/**
 * \file
 * \author Christien Alden (34119981)
 * \brief Internal header for the OFF file format reader
 * \remark
 * This file contains internal declarations and should not be included directly;
 * include \ref alibrary/geometry/off.h instead. The file has been made available
 * for the purpose of documentation and testing.
 *
*/
#pragma once
#include <core/config.h>
#include <alibrary/geometry/off.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * \struct OFFReader
	*/
struct OFFReader
{
	jmp_buf errorContext;            ///< the error handling context
	FILE* stream;                    ///< the file stream to read from
	size_t lineNumber;               ///< the current line number
	char line[MAX_LINE_LENGTH];      ///< holds the currently buffered line
	char* tokens[MAX_LINE_TOKENS];   ///< the tokens point to the line buffer
	size_t numTokens;                ///< the number of tokens in the line
	bool hasHeader;                  ///< true if the file has an OFF descriptor
	OFF* obj;                        ///< the object being created
};

// setup and teardown functions
INTERNAL void RemovePreamble(struct OFFReader* reader);
INTERNAL void InitialiseOFF(struct OFFReader* reader);
INTERNAL void ReadCounts(struct OFFReader* reader);
INTERNAL void AllocateVertices(struct OFFReader* reader);
INTERNAL void AllocateFaces(struct OFFReader* reader);
INTERNAL void ReadVertices(struct OFFReader* reader);
INTERNAL void ReadFaces(struct OFFReader* reader);
INTERNAL void DestroyOFFFace(struct OFFFace* face);

// utility functions
INTERNAL void HandleError(jmp_buf buf, const char* fmt, ...);
INTERNAL bool SeekToNextLine(struct OFFReader* reader);
INTERNAL bool ShouldIgnoreLine(const char* line);
INTERNAL void ValidateFileTail(struct OFFReader* reader);

#ifdef __cplusplus
}
#endif
