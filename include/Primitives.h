 /*
 * File:   Primitives.h
 * Author: Donald "Datriot" Whyte
 *
 * Created on February 28, 2009, 7:15 PM
 */

#ifndef PRIMITIVES_H
#define PRIMITIVES_H

namespace parcel
{

namespace graphics
{

    /* Enumerator which contains all the different types of primitives you can use. */
    enum PrimitiveType
    {
        PRIMITIVETYPE_POINT,
        PRIMITIVETYPE_LINE,
        PRIMITIVETYPE_LINESTRIP,
        PRIMITIVETYPE_LINELOOP,
        PRIMITIVETYPE_TRIANGLE,
        PRIMITIVETYPE_TRIANGLESTRIP,
        PRIMITIVETYPE_TRIANGLEFAN,
        PRIMITIVETYPE_QUAD,
        PRIMITIVETYPE_QUADSTRIP,
        PRIMITIVETYPE_POLYGON,
    };


    /* These structs hold indices to vertices. For example, triangle holds three indices
     * for three vertices that will form the triangle as well as three indices for texture
     * coordinates and normals.
     * They also use an union so the indices can be accessed using the . operator as
     * well as treating it like an array with the [] operator.*/
    struct Point
    {
        union
        {
            struct
            {
                int v1;
            };

            int values[1];
        };
    };

    struct Line
    {
        union
        {
            // Access via member properties
            struct
            {
                int v1, v2;
            };

            // Or access like an array
            int values[2];
        };
    };

    struct Triangle
    {
        union
        {
            struct
            {
                int v1, v2, v3;
            };

            int values[3];
        };
    };

    struct Quad
    {
        union
        {
            struct
            {
                int v1, v2, v3, v4;
            };

            int values[4];
        };
    };

}

}

#endif
