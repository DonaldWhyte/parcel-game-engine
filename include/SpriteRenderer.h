/*
 * File:   SpriteRenderer.h
 * Author: Donald "Datriot" Whyte
 *
 * Created on June 29, 2009, 6:45 PM
 */

#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include <GLee.h>
#include "ARenderer.h"
#include "RenderDevice.h"
#include "Util.h"

namespace parcel
{

namespace graphics
{

    /* This acts the same as VBORenderer, except renderables that draw
     * geometry do not use normals and 3D vertices, they use 2D vertices
     * instead. This means four floats (16 bytes) is saved for every vertex. */
    class SpriteRenderer : public ARenderer
    {


    private:

        GLuint vboID;
        float* vboData;
        std::vector<general::ArrayIndices> arrayIndices;
        /* Used to make sure geometry data exists before calling glDrawArrays
         * in the RenderObject() method. */
        int vboMemorySize;

        RenderDevice* renderDevice;


        void ProcessRenderable(IRenderable* renderable, unsigned int& vboIndex, unsigned int& vertexNumber);
        void RenderObject(IRenderable* renderable, unsigned int& index);


    public:

        SpriteRenderer(RenderDevice* renderDevice, debug::Logger* log, const bool& willDeleteAll);
        ~SpriteRenderer();

        void Update();
        void Render();


    };


}

}

#endif
