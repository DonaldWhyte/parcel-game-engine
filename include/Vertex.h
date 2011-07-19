/*
 * File:   Vertex.h
 * Author: Donald "Datriot" Whyte
 *
 * Created on February 20, 2009, 9:49 AM
 * Added SpriteVertex on June 29, 7:20 PM
 */

#ifndef VERTEX_H
#define VERTEX_H

#include "Vector.h"

namespace parcel
{

namespace graphics
{

    /* Stores the position, normal and texture coordinates for a single vertex.
     * NOTE: May decide to have multiple structs such as Vertex, TexturedVertex,
     * TexturedLitVertex, and LitVertex. */
    struct Vertex
    {

        maths::vector3f position; // The position of the vertex
        maths::vector2f texCoord; // The texture coordinates of the vertex
        maths::vector3f normal; // Vector perpendicular to the vertex, used for lighting


        Vertex() : position(maths::vector3f(0.0f, 0.0f, 0.0f)), texCoord(maths::vector2f(0.0f, 0.0f)),
            normal(maths::vector3f(0.0f, 0.0f, 0.0f))
        {
        }

        Vertex(const maths::vector3f& vertPosition, const maths::vector2f& vertTexCoord,
            const maths::vector3f& vertNormal) :
            position(vertPosition), texCoord(vertTexCoord), normal(vertNormal)
        {
        }

    };


    /* Used for 2D geometry that is textured. */
    struct SpriteVertex
    {

        maths::vector2f position; // The position of the vertex
        maths::vector2f texCoord; // The texture coordinates of the vertex


        SpriteVertex() : position(maths::vector2f(0.0f, 0.0f)), texCoord(maths::vector2f(0.0f, 0.0f))
        {
        }

        SpriteVertex(const maths::vector2f& vertPosition, const maths::vector2f& vertTexCoord) :
            position(vertPosition), texCoord(vertTexCoord)
        {
        }

    };


}

}

#endif
