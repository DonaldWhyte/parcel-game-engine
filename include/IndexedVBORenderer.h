/*
 * File:   IndexedVBORenderer.h
 * Author: Donald "Datriot" Whyte
 *
 * Created on March 28, 2009, 10:02 AM
 */

#ifndef INDEXEDVBORENDERER_H
#define INDEXEDVBORENDERER_H

#include <GLee.h>
#include "ARenderer.h"
#include "RenderDevice.h"
#include "Util.h"

namespace parcel
{

    namespace graphics
    {

        /* This is a lot like VBORenderer, except it has holds a normal data VBO and
         * an element VBO, which holds the indexes to the data. This can be used for
         * 3D models and other objects that use triangle/face lists to save memory
         * wastage on reused vertices. */
        class IndexedVBORenderer : public ARenderer
        {


        private:

            /* IDs to the data and index (element) vertex buffer objects. */
            GLuint dataVBO, indexVBO;

            float* vboData; // Used to store a pointer to the data VBO
            int* indexVBOData; // Used to store a pointer to the index VBO

            /* Used to store every renderable's start and end indices for the INDEX VBO.
             * This is unlike the standard VBORenderer, which stores indices to render
             * the raw data by itself. This one, however, stores the indices in the ELEMENT
             * array, wich holds indices for the vertex data. arrayIndices is used with
             * glDrawElements. */
            std::vector<general::ArrayIndices> arrayIndices;

            RenderDevice* renderDevice; // Used for activating a renderable's skin and texture


            /* Processes one renderable, adding its data to the VBO.
             * It takes in both VBO's current index and the number of vertices
             * and faces already processed. */
            void ProcessRenderable(IRenderable* renderable, unsigned int& dataVBOIndex,
                unsigned int& elementVBOIndex, unsigned int& triangleNumber);
            /* Renders a single object by calling glDrawArrays/glDrawElements, activating the
             * skin and transforming objects by their matrix. */
            void RenderObject(IRenderable* renderable, unsigned int& index);


        public:

            /* Initialises properties and creates two blank VBOs. */
            IndexedVBORenderer(RenderDevice* renderDevice, debug::Logger* log, const bool& willDeleteAll);
            /* Destructor, deletes both VBOs. */
            ~IndexedVBORenderer();

            /* Updates both the data and index VBOs as well as the arrayIndices vector. */
            void Update();
            /* This binds both VBOs and renders every renderable in the list. */
            void Render();


        };

    }

}
#endif
