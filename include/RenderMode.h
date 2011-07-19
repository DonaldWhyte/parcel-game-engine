/*
 * File:   RenderMode.h
 * Author: Donald "Datriot" Whyte
 *
 * Created on January 12, 2009, 5:37 PM
 */

#ifndef RENDERMODE_H
#define RENDERMODE_H

namespace parcel
{

namespace graphics
{


    /* Determines mode of the render device. With 2D, two dimensional matrices,
     * projection and so on are used. With 3D, three dimensional matrices are used
     * with (usually) perspective projection. If mode is switched, the RenderDevice
     * is returned to its default state (meaning matrices are reset, state of graphics
     * API reset and so on. */
    enum RenderMode
    {
        RENDERMODE_2D = 0,
        RENDERMODE_3D,
    };

    /* Determines the type of vertex being passed to and from the RenderDevice.
     * This helps in determining which states to enable/disable and what operations
     * are needed to be performed. UNUSED */
    enum VertexType
    {
        VERTEXTYPE_UNTRANSFORMED_UNLIT,
        VERTEXTYPE_TRANSFORMED_UNLIT,
        VERTEXTYPE_UNTRANSFORMED_LIT,
        VERTEXTYPE_TRANSFORMED_LIT,
    };


}

}

#endif
