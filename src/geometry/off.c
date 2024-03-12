/**
 * \file
 * \author Christien Alden (34119981)
 * \brief OFF file format internal functions and structures.
*/
#include <alibrary/geometry/off.h>
#include <alibrary/math/vec3.h>
#include <alibrary/core/error.h>
#include <alibrary/core/string.h>
#include <assert.h>
#include <ctype.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
static void RemovePreamble(struct OFFReader* reader);
static void InitialiseOFF(struct OFFReader* reader);
static void ReadCounts(struct OFFReader* reader);
static void AllocateVertices(struct OFFReader* reader);
static void AllocateFaces(struct OFFReader* reader);
static void ReadVertices(struct OFFReader* reader);
static void ReadFaces(struct OFFReader* reader);
static void DestroyOFFFace(struct OFFFace* face);

// utility functions
static void HandleError(jmp_buf buf, const char* fmt, ...);
static bool SeekToNextLine(struct OFFReader* reader);
static bool ShouldIgnoreLine(const char* line);
static void ValidateFileTail(struct OFFReader* reader);


//--------------------------------------------------------------------------------------------------
// Public Functions
//--------------------------------------------------------------------------------------------------

OFF* OFFImportFile(FILE* stream)
{
	if (stream == NULL)
	{
		WriteGlobalErrorMessage("The passed FILE* argument 'stream' was NULL");
		return NULL;
	}

	// setup OFFReader object
	struct OFFReader reader = {
		.stream = stream,
		.lineNumber = 0,
		.hasHeader = false,
		.obj = NULL
	};

	// setup error handling context
	if (setjmp(reader.errorContext) == 0)
	{
		InitialiseOFF(&reader);
		RemovePreamble(&reader);
		ReadCounts(&reader);
		AllocateVertices(&reader);
		AllocateFaces(&reader);
		ReadVertices(&reader);
		ReadFaces(&reader);
		ValidateFileTail(&reader);
	}
	else // cleanup resources if an error occurs
	{
		// this will set the pointer to NULL
		OFFDestroy(&reader.obj);
	}

	return reader.obj;
}

void OFFExportFile(OFF* obj, FILE* stream, int precision)
{
	fprintf(stream, "OFF\n");
	fprintf(stream, "%d %d %d\n", obj->numVertices, obj->numFaces, obj->numEdges);
	if (obj->numVertices > 0)
	{
		for (uint32_t vi = 0; vi < obj->numVertices; ++vi)
		{
			Vec3* vertex = &obj->vertices[vi];
			fprintf(stream, "%.*f %.*f %.*f\n",
				precision,
				vertex->x,
				precision,
				vertex->y,
				precision,
				vertex->z
			);
		}
	}

	if (obj->numFaces > 0)
	{
		for (uint32_t fi = 0; fi < obj->numFaces; ++fi)
		{
			struct OFFFace* face = &obj->faces[fi];
			fprintf(stream, "%d", face->type);

			for (int vi = 0; vi < (int) face->type; ++vi)
			{
				fprintf(stream, " %d", face->vertices[vi]);
			}
			fprintf(stream, "\n");
		}
	}
}

void OFFDestroy(OFF** obj)
{
	if ((obj == NULL) || (*obj == NULL))
	{
		return;
	}

	// destroy the faces and vertices
	for (uint32_t fi = 0; fi < (*obj)->numFaces; ++fi)
	{
		struct OFFFace* face = &(*obj)->faces[fi];
		DestroyOFFFace(face);
	}
	free((*obj)->faces);
	free((*obj)->vertices);

	// reset fields
	(*obj)->faces = NULL;
	(*obj)->vertices = NULL;
	(*obj)->numVertices = 0;
	(*obj)->numFaces = 0;
	(*obj)->numEdges = 0;

	// destroy the base object and reset the pointer
	free(*obj);
	*obj = NULL;
}

void OFFNormalise(OFF* obj, bool alignToOrigin)
{
	if (!obj)
	{
		return;
	}

	size_t numVertices = obj->numVertices;
	if (numVertices == 0)
	{
		return;
	}

	// find the centroid of the mesh
	Vec3 centroid = { 0.0f, 0.0f, 0.0f };
	Vec3* vertices = obj->vertices;
	for (uint32_t vi = 0; vi < numVertices; ++vi)
	{
		centroid = Vec3Add(&centroid, &vertices[vi]);
	}
	centroid = Vec3Scale(&centroid, 1.0f / numVertices);

	// translate the vertices to the origin
	for (uint32_t vi = 0; vi < numVertices; ++vi)
	{
		vertices[vi] = Vec3Sub(&vertices[vi], &centroid);
	}

	// find the maximum vertex magnitude
	float maxMagnitude = Vec3Lenth(vertices);
	for (uint32_t vi = 1; vi < numVertices; ++vi)
	{
		float magnitude = Vec3Lenth(&vertices[vi]);
		if (magnitude > maxMagnitude)
		{
			maxMagnitude = magnitude;
		}
	}

	// normalise the vertices based on the maximum magnitude
	float scale = 1.0f / maxMagnitude;
	for (uint32_t vi = 0; vi < numVertices; ++vi)
	{
		vertices[vi] = Vec3Scale(&vertices[vi], scale);
	}

	// allows for the mesh itself to fit within the unit sphere if the sphere is
	// aligned to the mesh's local coordinate system
	if (!alignToOrigin)
	{
		// translate the vertices back to the centroid
		for (uint32_t vi = 0; vi < numVertices; ++vi)
		{
			vertices[vi] = Vec3Add(&vertices[vi], &centroid);
		}
	}
}


//--------------------------------------------------------------------------------------------------
// Setup and Teardown
//--------------------------------------------------------------------------------------------------

void RemovePreamble(struct OFFReader* reader)
{
	// read the first line of the file
	if (!SeekToNextLine(reader))
	{
		HandleError(reader->errorContext, "OFF : File is empty");
	}

	// check if the line is the OFF descriptor
	reader->numTokens = tokeniseLine(reader->line, reader->tokens, MAX_LINE_TOKENS);
	if (reader->numTokens == 0)
	{
		// this should never happen...
		HandleError(
			reader->errorContext,
			"OFF : tokeniseLine failed when reading OFF descriptor on line %d, contact the developer",
			reader->lineNumber
		);
	}

	if (!isdigit(*reader->tokens[0]))
	{
		if (strcmp(reader->tokens[0], "OFF") == 0)
		{
			reader->hasHeader = true;

			// check that trailing characters are commented out
			if (reader->numTokens > 1 && *reader->tokens[1] != '#')
			{
				HandleError(
					reader->errorContext,
					"OFF : Unexpected characters after OFF descriptor on line %d",
					reader->lineNumber
				);
			}
		}
		else
		{
			HandleError(
				reader->errorContext,
				"OFF : Expected OFF descriptor on line %d",
				reader->lineNumber
			);
		}
	}
}

void ReadCounts(struct OFFReader* reader)
{
	// if the last buffered line was the header, we need to read the next line
	if (reader->hasHeader)
	{
		if (!SeekToNextLine(reader))
		{
			HandleError(
				reader->errorContext,
				"OFF : Unexpected EOF after OFF descriptor"
			);
		}

		reader->numTokens = tokeniseLine(reader->line, reader->tokens, MAX_LINE_TOKENS);
	}

	// there needs to be at least 3 tokens for the vertex, face, and edge counts
	if (reader->numTokens < 3)
	{
		HandleError(
			reader->errorContext,
			"OFF : Expected 3 integers on line %d",
			reader->lineNumber
		);
	}

	// attempt to convert each token to an integer and store the results
	OFF* obj = reader->obj;
	if (!strToUint32(reader->tokens[0], &obj->numVertices, 10))
	{
		HandleError(
			reader->errorContext,
			"OFF : Failed to parse vertex count on line %d",
			reader->lineNumber
		);
	}

	if (!strToUint32(reader->tokens[1], &obj->numFaces, 10))
	{
		HandleError(
			reader->errorContext,
			"OFF : Failed to parse face count on line %d",
			reader->lineNumber
		);
	}

	if (!strToUint32(reader->tokens[2], &obj->numEdges, 10))
	{
		HandleError(
			reader->errorContext,
			"OFF : Failed to parse edge count on line %d",
			reader->lineNumber
		);
	}

	// check that trailing characters are commented out
	if (reader->numTokens > 3 && *reader->tokens[3] != '#')
	{
		HandleError(
			reader->errorContext,
			"OFF : Unexpected characters after counts on line %d",
			reader->lineNumber
		);
	}

	return;
}

void InitialiseOFF(struct OFFReader* reader)
{
	reader->obj = (OFF*) malloc(sizeof(OFF));
	if (reader->obj == NULL)
	{
		HandleError(
			reader->errorContext,
			"OFF : Failed to allocate memory for the OFF object"
		);
	}

	// initialise the object fields
	OFF* obj = reader->obj;
	obj->numVertices = 0;
	obj->numFaces = 0;
	obj->numEdges = 0;
	obj->vertices = NULL;
	obj->faces = NULL;
}

void AllocateVertices(struct OFFReader* reader)
{
	OFF* obj = reader->obj;
	obj->vertices = (Vec3*) malloc(sizeof(Vec3) * obj->numVertices);
	if (obj->vertices == NULL)
	{
		HandleError(
			reader->errorContext,
			"OFF : Failed to allocate memory for vertices"
		);
	}

	for (uint32_t vi = 0; vi < obj->numVertices; ++vi)
	{
		obj->vertices[vi] = (Vec3) { 0.0f, 0.0f, 0.0f };
	}
}

void AllocateFaces(struct OFFReader* reader)
{
	OFF* obj = reader->obj;
	obj->faces = (struct OFFFace*) malloc(sizeof(struct OFFFace) * obj->numFaces);
	if (obj->faces == NULL)
	{
		HandleError(
			reader->errorContext,
			"OFF : Failed to allocate memory for faces"
		);
	}

	for (uint32_t fi = 0; fi < obj->numFaces; ++fi)
	{
		struct OFFFace* face = &obj->faces[fi];
		face->vertices = NULL;
		face->type = UNDEFINED;
	}
}

void ReadVertices(struct OFFReader* reader)
{
	OFF* obj = reader->obj;

	for (uint32_t vi = 0; vi < obj->numVertices; ++vi)
	{
		if (!SeekToNextLine(reader))
		{
			HandleError(
				reader->errorContext,
				"OFF : Unexpected end of file when reading vertices");
		}

		// tokenise the line and check that there are at least 3 tokens
		reader->numTokens = tokeniseLine(reader->line, reader->tokens, MAX_LINE_TOKENS);
		if (reader->numTokens < 3)
		{
			HandleError(
				reader->errorContext,
				"OFF : Expected 3 floats (vertex) on line %d",
				reader->lineNumber
			);
		}

		// convert and validate the vertex coordinates
		Vec3* vertex = &obj->vertices[vi];
		if (!strToFloat(reader->tokens[0], &vertex->x))
		{
			HandleError(
				reader->errorContext,
				"OFF : Failed to parse x coordinate on line %d",
				reader->lineNumber
			);
		}

		if (!strToFloat(reader->tokens[1], &vertex->y))
		{
			HandleError(
				reader->errorContext,
				"OFF : Failed to parse y coordinate on line %d",
				reader->lineNumber
			);
		}

		if (!strToFloat(reader->tokens[2], &vertex->z))
		{
			HandleError(
				reader->errorContext,
				"OFF : Failed to parse z coordinate on line %d",
				reader->lineNumber
			);
		}

		// check that trailing characters are commented out
		if (reader->numTokens > 3 && *reader->tokens[3] != '#')
		{
			HandleError(
				reader->errorContext,
				"OFF : Unexpected characters after vertex on line %d",
				reader->lineNumber
			);
		}
	}
}

void ReadFaces(struct OFFReader* reader)
{
	OFF* obj = reader->obj;

	for (uint32_t fi = 0; fi < obj->numFaces; ++fi)
	{
		if (!SeekToNextLine(reader))
		{
			HandleError(
				reader->errorContext,
				"OFF : Unexpected end of file when reading faces"
			);
		}

		// tokenise the line and check that there are at least 4 tokens
		reader->numTokens = tokeniseLine(reader->line, reader->tokens, MAX_LINE_TOKENS);
		if (reader->numTokens < 4)
		{
			HandleError(
				reader->errorContext,
				"OFF : Expected at least 4 integers (face) on line %d",
				reader->lineNumber
			);
		}

		// convert and validate the number of vertices in the face, must be 3 or 4
		uint32_t numVertices;
		if (!strToUint32(reader->tokens[0], &numVertices, 10) || numVertices < 3 || numVertices > 4)
		{
			HandleError(
				reader->errorContext,
				"OFF : Failed to parse vertex count on line %d",
				reader->lineNumber
			);
		}

		// set the type of the face and allocate memory for the vertices
		struct OFFFace* face = &obj->faces[fi];
		face->type = (enum OFFFaceType) numVertices;
		face->vertices = (uint32_t*) malloc(sizeof(uint32_t) * numVertices);

		// attempt to convert each token to an integer and store the results
		for (uint32_t vi = 0; vi < numVertices; ++vi)
		{
			uint32_t* vertex = &face->vertices[vi];
			if (!strToUint32(reader->tokens[vi + 1], vertex, 10))
			{
				HandleError(
					reader->errorContext,
					"OFF : Failed to parse vertex index on line %d",
					reader->lineNumber
				);
			}
		}

		// check that trailing characters are commented out
		if (reader->numTokens > (numVertices + 1) && *reader->tokens[numVertices + 1] != '#')
		{
			HandleError(
				reader->errorContext,
				"OFF : Unexpected characters after face on line %d",
				reader->lineNumber
			);
		}
	}
}

void DestroyOFFFace(struct OFFFace* face)
{
	if (face == NULL)
	{
		return;
	}

	// destroy the vertices
	free(face->vertices);

	// reset fields
	face->vertices = NULL;
	face->type = UNDEFINED;
}


//--------------------------------------------------------------------------------------------------
// Utilities
//--------------------------------------------------------------------------------------------------

void HandleError(jmp_buf buf, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	WriteGlobalErrorMessageVA(fmt, args);
	va_end(args);
	longjmp(buf, 1);
}

bool SeekToNextLine(struct OFFReader* file)
{
	do
	{
		file->lineNumber++;
		if (fgets(file->line, MAX_LINE_LENGTH, file->stream) == NULL)
		{
			return false;
		}
	} while (ShouldIgnoreLine(file->line));

	removeNewlineChar(file->line);
	return true;
}

bool ShouldIgnoreLine(const char* line)
{
	// parse through the line until we find a non-whitespace character
	const char* p = line;
	while (*p == ' ' || *p == '\t')
	{
		p++;
	}

	// ignore the line if the first non-whitespace character is:
	// a comment, carriage return, or newline
	return *p == '#' || *p == '\r' || *p == '\n';
}

void ValidateFileTail(struct OFFReader* reader)
{
	// check for extra invalid lines at end of file
	if (SeekToNextLine(reader))
	{
		HandleError(
			reader->errorContext,
			"OFF : Unexpected characters after faces on line %d",
			reader->lineNumber
		);
	}
}
