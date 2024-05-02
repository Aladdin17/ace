/**
 * \file
 * \author Christien Alden
 * \brief OFF functions and structures implementation.
 */
#include "off.h"
#include <ace/core/error.h>
#include <ace/core/string.h>
#include <ace/math/vec3.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef __STD_LIB_EXT1__
    // use the secure CRT functions if available
    #define fprintf fprintf_s
#else
    // ignore the use of depracated functions if secure CRT is not available
    // NOLINTNEXTLINE(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
    #define fprintf fprintf
#endif

// the base being used for the uint conversion
static const int off_uint_base = 10;

//--------------------------------------------------------------------------------------------------
// Public Functions
//--------------------------------------------------------------------------------------------------

OFF* OFFImportFile(FILE* stream)
{
    if ( stream == NULL )
    {
        WriteGlobalErrorMessage("The passed FILE* argument 'stream' was NULL");
        return NULL;
    }

    // setup OFFReader object
    OFFReader reader = { .stream = stream, .lineNumber = 0, .hasHeader = false, .obj = NULL };

    // setup the function calls
    typedef bool (*OFFReaderFunction)(OFFReader*);
    static const OFFReaderFunction readerFunc[] = { InitialiseOFF,    RemovePreamble,  ReadCounts,
                                                    AllocateVertices, AllocateFaces,   ReadVertices,
                                                    ReadFaces,        ValidateFileTail };

    // call each function in the array
    const size_t numFunctions = sizeof(readerFunc) / sizeof(OFFReaderFunction);
    bool         parseFailed  = false;
    for ( size_t fi = 0; fi < numFunctions && !parseFailed; ++fi )
    {
        // func returns true if on success so we must negate this
        OFFReaderFunction func = readerFunc[fi];
        parseFailed            = !func(&reader);
    }

    // cleanup resources if an error occurs
    if ( parseFailed )
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
    if ( obj->numVertices > 0 )
    {
        for ( uint32_t vi = 0; vi < obj->numVertices; ++vi )
        {
            ac_vec3* vertex = &obj->vertices[vi];
            fprintf(
                stream,
                "%.*f %.*f %.*f\n",
                precision,
                vertex->x,
                precision,
                vertex->y,
                precision,
                vertex->z
            );
        }
    }

    if ( obj->numFaces > 0 )
    {
        for ( uint32_t fi = 0; fi < obj->numFaces; ++fi )
        {
            OFFFace* face = &obj->faces[fi];
            fprintf(stream, "%d", face->type);

            for ( int vi = 0; vi < (int) face->type; ++vi )
            {
                fprintf(stream, " %d", face->vertices[vi]);
            }
            fprintf(stream, "\n");
        }
    }
}

void OFFDestroy(OFF** obj)
{
    if ( (obj == NULL) || (*obj == NULL) )
    {
        return;
    }

    // destroy the faces and vertices
    for ( uint32_t fi = 0; fi < (*obj)->numFaces; ++fi )
    {
        OFFFace* face = &(*obj)->faces[fi];
        DestroyOFFFace(face);
    }
    free((*obj)->faces);
    free((*obj)->vertices);

    // reset fields
    (*obj)->faces       = NULL;
    (*obj)->vertices    = NULL;
    (*obj)->numVertices = 0;
    (*obj)->numFaces    = 0;
    (*obj)->numEdges    = 0;

    // destroy the base object and reset the pointer
    free(*obj);
    *obj = NULL;
}

void OFFNormalise(OFF* obj, bool alignToOrigin)
{
    if ( !obj )
    {
        return;
    }

    size_t numVertices = obj->numVertices;
    if ( numVertices == 0 )
    {
        return;
    }

    // find the centroid of the mesh
    ac_vec3  centroid = { 0.0f, 0.0f, 0.0f };
    ac_vec3* vertices = obj->vertices;
    for ( uint32_t vi = 0; vi < numVertices; ++vi )
    {
        centroid = ac_vec3_add(&centroid, &vertices[vi]);
    }
    centroid = ac_vec3_scale(&centroid, 1.0f / (float) numVertices);

    // translate the vertices to the origin
    for ( uint32_t vi = 0; vi < numVertices; ++vi )
    {
        vertices[vi] = ac_vec3_sub(&vertices[vi], &centroid);
    }

    // find the maximum vertex magnitude
    float maxMagnitude = ac_vec3_magnitude(vertices);
    for ( uint32_t vi = 1; vi < numVertices; ++vi )
    {
        float magnitude = ac_vec3_magnitude(&vertices[vi]);
        if ( magnitude > maxMagnitude )
        {
            maxMagnitude = magnitude;
        }
    }

    // normalise the vertices based on the maximum magnitude
    float scale = 1.0f / maxMagnitude;
    for ( uint32_t vi = 0; vi < numVertices; ++vi )
    {
        vertices[vi] = ac_vec3_scale(&vertices[vi], scale);
    }

    // allows for the mesh itself to fit within the unit sphere if the sphere is
    // aligned to the mesh's local coordinate system
    if ( !alignToOrigin )
    {
        // translate the vertices back to the centroid
        for ( uint32_t vi = 0; vi < numVertices; ++vi )
        {
            vertices[vi] = ac_vec3_add(&vertices[vi], &centroid);
        }
    }
}

//--------------------------------------------------------------------------------------------------
// Setup and Teardown
//--------------------------------------------------------------------------------------------------

bool RemovePreamble(OFFReader* reader)
{
    // read the first line of the file
    if ( !SeekToNextLine(reader) )
    {
        WriteGlobalErrorMessage("OFF : File is empty");
        return false;
    }

    // check if the line is the OFF descriptor
    reader->numTokens = tokeniseString(reader->line, reader->tokens, OFF_MAX_LINE_TOKENS, " \t");
    if ( reader->numTokens == 0 )
    {
        WriteGlobalErrorMessage(
            "OFF : tokeniseString failed when reading OFF descriptor on line %d, contact the "
            "developer",
            reader->lineNumber
        );
        return false;
    }

    if ( !isdigit(*reader->tokens[0]) )
    {
        if ( strcmp(reader->tokens[0], "OFF") == 0 )
        {
            reader->hasHeader = true;

            // check that trailing characters are commented out
            if ( reader->numTokens > 1 && *reader->tokens[1] != '#' )
            {
                WriteGlobalErrorMessage(
                    "OFF : Unexpected characters after OFF descriptor on line %d",
                    reader->lineNumber
                );
                return false;
            }
        }
        else
        {
            WriteGlobalErrorMessage("OFF : Expected OFF descriptor on line %d", reader->lineNumber);
            return false;
        }
    }

    return true;
}

bool ReadCounts(OFFReader* reader)
{
    // if the last buffered line was the header, we need to read the next line
    if ( reader->hasHeader )
    {
        if ( !SeekToNextLine(reader) )
        {
            WriteGlobalErrorMessage("OFF : Unexpected EOF after OFF descriptor");
            return false;
        }

        reader->numTokens =
            tokeniseString(reader->line, reader->tokens, OFF_MAX_LINE_TOKENS, " \t");
    }

    // there needs to be at least 3 tokens for the vertex, face, and edge counts
    if ( reader->numTokens < 3 )
    {
        WriteGlobalErrorMessage("OFF : Expected 3 integers on line %d", reader->lineNumber);
        return false;
    }

    // attempt to convert each token to an integer and store the results
    OFF* obj = reader->obj;
    if ( !strToUint32(reader->tokens[0], &obj->numVertices, off_uint_base) )
    {
        WriteGlobalErrorMessage(
            "OFF : Failed to parse vertex count on line %d",
            reader->lineNumber
        );
        return false;
    }

    if ( !strToUint32(reader->tokens[1], &obj->numFaces, off_uint_base) )
    {
        WriteGlobalErrorMessage("OFF : Failed to parse face count on line %d", reader->lineNumber);
        return false;
    }

    if ( !strToUint32(reader->tokens[2], &obj->numEdges, off_uint_base) )
    {
        WriteGlobalErrorMessage("OFF : Failed to parse edge count on line %d", reader->lineNumber);
        return false;
    }

    // check that trailing characters are commented out
    if ( reader->numTokens > 3 && *reader->tokens[3] != '#' )
    {
        WriteGlobalErrorMessage(
            "OFF : Unexpected characters after counts on line %d",
            reader->lineNumber
        );
        return false;
    }

    return true;
}

bool InitialiseOFF(OFFReader* reader)
{
    reader->obj = (OFF*) malloc(sizeof(OFF));
    if ( reader->obj == NULL )
    {
        WriteGlobalErrorMessage("OFF : Failed to allocate memory for the OFF object");
        return false;
    }

    // initialise the object fields
    OFF* obj         = reader->obj;
    obj->numVertices = 0;
    obj->numFaces    = 0;
    obj->numEdges    = 0;
    obj->vertices    = NULL;
    obj->faces       = NULL;

    return true;
}

bool AllocateVertices(OFFReader* reader)
{
    OFF* obj      = reader->obj;
    obj->vertices = (ac_vec3*) malloc(sizeof(ac_vec3) * obj->numVertices);
    if ( obj->vertices == NULL )
    {
        WriteGlobalErrorMessage("OFF : Failed to allocate memory for vertices");
        return false;
    }

    for ( uint32_t vi = 0; vi < obj->numVertices; ++vi )
    {
        obj->vertices[vi] = (ac_vec3){ 0.0f, 0.0f, 0.0f };
    }

    return true;
}

bool AllocateFaces(OFFReader* reader)
{
    OFF* obj   = reader->obj;
    obj->faces = (OFFFace*) malloc(sizeof(OFFFace) * obj->numFaces);
    if ( obj->faces == NULL )
    {
        WriteGlobalErrorMessage("OFF : Failed to allocate memory for faces");
        return false;
    }

    for ( uint32_t fi = 0; fi < obj->numFaces; ++fi )
    {
        OFFFace* face  = &obj->faces[fi];
        face->vertices = NULL;
        face->type     = OFFFaceType_UNDEFINED;
    }

    return true;
}

bool ReadVertices(OFFReader* reader)
{
    OFF* obj = reader->obj;

    for ( uint32_t vi = 0; vi < obj->numVertices; ++vi )
    {
        if ( !SeekToNextLine(reader) )
        {
            WriteGlobalErrorMessage("OFF : Unexpected end of file when reading vertices");
            return false;
        }

        // tokenise the line and check that there are at least 3 tokens
        reader->numTokens =
            tokeniseString(reader->line, reader->tokens, OFF_MAX_LINE_TOKENS, " \t");
        if ( reader->numTokens < 3 )
        {
            WriteGlobalErrorMessage(
                "OFF : Expected 3 floats (vertex) on line %d",
                reader->lineNumber
            );
            return false;
        }

        // convert and validate the vertex coordinates
        ac_vec3* vertex = &obj->vertices[vi];
        if ( !strToFloat(reader->tokens[0], &vertex->x) )
        {
            WriteGlobalErrorMessage(
                "OFF : Failed to parse x coordinate on line %d",
                reader->lineNumber
            );
            return false;
        }

        if ( !strToFloat(reader->tokens[1], &vertex->y) )
        {
            WriteGlobalErrorMessage(
                "OFF : Failed to parse y coordinate on line %d",
                reader->lineNumber
            );
            return false;
        }

        if ( !strToFloat(reader->tokens[2], &vertex->z) )
        {
            WriteGlobalErrorMessage(
                "OFF : Failed to parse z coordinate on line %d",
                reader->lineNumber
            );
            return false;
        }

        // check that trailing characters are commented out
        if ( reader->numTokens > 3 && *reader->tokens[3] != '#' )
        {
            WriteGlobalErrorMessage(
                "OFF : Unexpected characters after vertex on line %d",
                reader->lineNumber
            );
            return false;
        }
    }

    return true;
}

bool ReadFaces(OFFReader* reader)
{
    OFF* obj = reader->obj;

    for ( uint32_t fi = 0; fi < obj->numFaces; ++fi )
    {
        if ( !SeekToNextLine(reader) )
        {
            WriteGlobalErrorMessage("OFF : Unexpected end of file when reading faces");
            return false;
        }

        // tokenise the line and check that there are at least 4 tokens
        reader->numTokens =
            tokeniseString(reader->line, reader->tokens, OFF_MAX_LINE_TOKENS, " \t");
        if ( reader->numTokens < 4 )
        {
            WriteGlobalErrorMessage(
                "OFF : Expected at least 4 integers (face) on line %d",
                reader->lineNumber
            );
            return false;
        }

        // convert and validate the number of vertices in the face
        uint32_t numVertices;
        if ( !strToUint32(reader->tokens[0], &numVertices, off_uint_base) ||
             numVertices < OFFFaceType_MIN || numVertices > OFFFaceType_MAX )
        {
            WriteGlobalErrorMessage(
                "OFF : Failed to parse vertex count on line %d",
                reader->lineNumber
            );
            return false;
        }

        // set the type of the face and allocate memory for the vertices
        OFFFace* face  = &obj->faces[fi];
        face->type     = (OFFFaceType) numVertices;
        face->vertices = (uint32_t*) malloc(sizeof(uint32_t) * numVertices);

        // attempt to convert each token to an integer and store the results
        for ( uint32_t vi = 0; vi < numVertices; ++vi )
        {
            uint32_t* vertex = &face->vertices[vi];
            if ( !strToUint32(reader->tokens[vi + 1], vertex, off_uint_base) )
            {
                WriteGlobalErrorMessage(
                    "OFF : Failed to parse vertex index on line %d",
                    reader->lineNumber
                );
                return false;
            }
        }

        // check that trailing characters are commented out
        if ( reader->numTokens > (numVertices + 1) && *reader->tokens[numVertices + 1] != '#' )
        {
            WriteGlobalErrorMessage(
                "OFF : Unexpected characters after face on line %d",
                reader->lineNumber
            );
            return false;
        }
    }

    return true;
}

void DestroyOFFFace(OFFFace* face)
{
    if ( face == NULL )
    {
        return;
    }

    // destroy the vertices
    free(face->vertices);

    // reset fields
    face->vertices = NULL;
    face->type     = OFFFaceType_UNDEFINED;
}

//--------------------------------------------------------------------------------------------------
// Utilities
//--------------------------------------------------------------------------------------------------

bool SeekToNextLine(OFFReader* file)
{
    do
    {
        file->lineNumber++;
        if ( fgets(file->line, OFF_MAX_LINE_LENGTH, file->stream) == NULL )
        {
            return false;
        }
    } while ( ShouldIgnoreLine(file->line) );

    removeNewlineChar(file->line);
    return true;
}

bool ShouldIgnoreLine(const char* line)
{
    // parse through the line until we find a non-whitespace character
    const char* cp = line;
    while ( *cp == ' ' || *cp == '\t' )
    {
        cp++;
    }

    // ignore the line if the first non-whitespace character is:
    // a comment, carriage return, or newline
    return *cp == '#' || *cp == '\r' || *cp == '\n';
}

bool ValidateFileTail(OFFReader* reader)
{
    // check for extra invalid lines at end of file
    if ( SeekToNextLine(reader) )
    {
        WriteGlobalErrorMessage(
            "OFF : Unexpected characters after faces on line %d",
            reader->lineNumber
        );
        return false;
    }

    return true;
}
