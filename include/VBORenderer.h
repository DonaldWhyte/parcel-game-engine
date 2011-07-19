/*
 * File:   VBORenderer.h
 * Author: Donald "Datriot" Whyte
 *
 * Created on February 17, 2009, 10:16 AM
 */

#ifndef VBORENDERER_H
#define VBORENDERER_H

#include <GLee.h>
#include "ARenderer.h"
#include "RenderDevice.h"
#include "Util.h"

namespace parcel
{

namespace graphics
{

    /* Provides storing and batch rendering functionality using Vertex Buffer Objects (VBOs)
     * within OpenGL. */
    class VBORenderer : public ARenderer
    {


    private:

        GLuint vboID; // ID for the renderer's Vertex Buffer Object (VBO)
        float* vboData; // Used to store a pointer the VBO's data

        // Used to store every renderable's start and end indices in the VBO
        std::vector<general::ArrayIndices> arrayIndices;

        RenderDevice* renderDevice; // Used for activating a renderable's skin and texture


        /* Processes one renderable, adding its data to the VBO.
         * It takes in the current VBO index and the number of vertices already processed. */
        void ProcessRenderable(IRenderable* renderable, unsigned int& vboIndex, unsigned int& vertexNumber);
        /* Renders a single object by calling glDrawArrays, activating the skin and
         * transforming objects by their matrix. */
        void RenderObject(IRenderable* renderable, unsigned int& index);


    public:

        /* The constructor initialises the properties and creates a blank VBO. */
        VBORenderer(RenderDevice* renderDevice, debug::Logger* log, const bool& willDeleteAll);
        /* The destructor destroys the VBO and releases any memory stored in vboData.
         * There is no need to delete every renderable, the superclass' (ARenderer) destructor
         * does that for us.*/
        ~VBORenderer();

        /* The update method clears the VBO and refills it with the renderable's values. Also
         * clears the arrayIndices std::vector and refills it with elements. */
        void Update();
        /* The render method renders every object by calling RenderObject() for each one.
         * DO NOT add/delete any renderables between the calls to Update() and Render(). */
        void Render();


    };

}

}

#endif
