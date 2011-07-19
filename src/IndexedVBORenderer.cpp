/*
 * File:   IndexedVBORenderer.cpp
 * Author: Donald "Datriot" Whyte
 *
 * Created on March 28, 2009, 10:12 AM
 */

#include "IndexedVBORenderer.h"

namespace parcel
{

    using namespace maths;

    namespace graphics
    {

        IndexedVBORenderer::IndexedVBORenderer(RenderDevice* renderDevice, debug::Logger* log,
            const bool& willDeleteAll) :
            // Initialiser list
            ARenderer(log, "IndexedVBORenderer", willDeleteAll), // Superclass constructor
            dataVBO(0), indexVBO(0),
            vboData(NULL), indexVBOData(NULL),
            renderDevice(renderDevice)
        {
            // Stores the currently bound buffers (or 0 for no buffer)
            int arrBuffer, elemBuffer;
            glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &arrBuffer);
            glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &elemBuffer);

            // Creates the data and index (element) VBOs and stores IDs in appropriate variables
            glGenBuffers(2, &dataVBO);
            // Data VBO
            glBindBuffer(GL_ARRAY_BUFFER, dataVBO);
            glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);
            // Index VBO
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);

            /* Makes sure to re-bind the buffers that were active BEFORE creating the buffers
             * for this renderer. */
            glBindBuffer(GL_ARRAY_BUFFER, arrBuffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elemBuffer);

            logger->WriteTextAndNewLine(logID, "IndexedVBORenderer created.");
        }


        IndexedVBORenderer::~IndexedVBORenderer()
        {
            // Deletes both VBOs
            glDeleteBuffers(2, &dataVBO);

            logger->WriteTextAndNewLine(logID, "IndexedVBORenderer destroyed.");
        }


        void IndexedVBORenderer::ProcessRenderable(IRenderable* renderable,
            unsigned int& dataVBOIndex, unsigned int& elementVBOIndex,
            unsigned int& triangleNumber)
        {
            /* Stores start and end of this renderable's triangles in the INDEX
             * (element) VBO array. */
            general::ArrayIndices indices;
            // Sets the start of the triangles
            indices.start = elementVBOIndex;

            unsigned int i; // Iterator for the loops
            unsigned int amount = 0; // The amount of triangles this renderable has to draw


            // If renderable holds geometry and triangle face data
            IIndexedGeometry* geometry = dynamic_cast<IIndexedGeometry*>(renderable);
            if (geometry)
            {
                // Gets the vertices from the renderable and fills the VBO with them
                const std::vector<Vertex>& vertexData = geometry->GetVertices();

                // Fills data VBO with the vertices
                for (i = 0; (i < vertexData.size()); i++)
                {
                    // Stores vertex position
                    vboData[dataVBOIndex++] = vertexData[i].position.x;
                    vboData[dataVBOIndex++] = vertexData[i].position.y;
                    vboData[dataVBOIndex++] = vertexData[i].position.z;
                    // Stores texture coordinates
                    vboData[dataVBOIndex++] = vertexData[i].texCoord.x;
                    vboData[dataVBOIndex++] = vertexData[i].texCoord.y;
                    // Stores normals
                    vboData[dataVBOIndex++] = vertexData[i].normal.x;
                    vboData[dataVBOIndex++] = vertexData[i].normal.y;
                    vboData[dataVBOIndex++] = vertexData[i].normal.z;
                }


                // Gets the triangles from the renderable
                const std::vector<Triangle>& triangleData = geometry->GetFaces();

                // Now fills the index VBO with the indices from the triangles
                for (i = 0; (i < triangleData.size()); i++)
                {
                    // Stores vertex indices
                    indexVBOData[elementVBOIndex++] = triangleData[i].v1;
                    indexVBOData[elementVBOIndex++] = triangleData[i].v2;
                    indexVBOData[elementVBOIndex++] = triangleData[i].v3;

                    // Increases the amount of indices, so it draws all three of the triangle's points
                    amount += 3;
                    // Also increases the total triangle number
                    triangleNumber++;
                }
            }

            // Processes all the group's renderables too
            IGroupRenderable* group = dynamic_cast<IGroupRenderable*>(renderable);
            if (group)
            {
                for (i = 0; (i < group->GetAmountOfRenderables()); i++)
                {
                    if (group->GetRenderable(i) != NULL)
                    {
                        ProcessRenderable(group->GetRenderable(i), dataVBOIndex, elementVBOIndex, triangleNumber);
                    }
                }
            }

            // Sets the amount of elements this object will draw to the amount of indices from this object
            indices.amount = amount;
            arrayIndices.push_back(indices);
        }


        void IndexedVBORenderer::Update()
        {
            // Gets the size the updated buffers will need to be
            int dataVBOMemorySize = 0;
            int indexVBOMemorySize = 0;

            /* Iterates through all renderables, adding their memory size (for vertex
             * data) to the total. */
            for (unsigned int i = 0; (i < renderables.size()); i++)
            {
                dataVBOMemorySize += renderables[i]->GetMemorySize();

                /* Checks if renderable contains indexed geometry. If it does, then it
                 * adds the memory size of all the renderable's triangles to total of
                 * the index VBO. */
                IIndexedGeometry* geometry  = dynamic_cast<IIndexedGeometry*>(renderables[i]);
                if (geometry)
                {
                    indexVBOMemorySize += geometry->GetTriangleMemorySize();
                }
            }


            // Binds the renderer's data and index buffers to make them active
            glBindBuffer(GL_ARRAY_BUFFER, dataVBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
            // Clears VBOs and specifies how the data in the arrays will be packed
            glBufferData(GL_ARRAY_BUFFER, dataVBOMemorySize, NULL, GL_DYNAMIC_DRAW);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexVBOMemorySize, NULL, GL_DYNAMIC_DRAW);

            /* If memory sizes of either data or indices is 0, just return since
             * there is nothing to update. */
            if (dataVBOMemorySize == 0 || indexVBOMemorySize == 0) return;

            // Gets pointers to data and index VBOs to put our values into
            vboData = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
            indexVBOData = (int*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);


            // Used for accessing data and index VBOs array's elements
            unsigned int dataVBOIndex = 0;
            unsigned int elementVBOIndex = 0;
            // Used for getting the start index for each renderable's ArrayIndices struct
            unsigned int triangleNumber = 0;

            // Clears the std::vector of its array indices and reserves needed amount of memory
            arrayIndices.clear();
            arrayIndices.reserve(renderables.size());

            /* Processes all the renderables, adding data to the VBO and putting indices to that
             * data into arrayIndices. */
            for (unsigned int i = 0; (i < renderables.size()); i++)
            {
                if (renderables[i] != NULL)
                {
                    ProcessRenderable(renderables[i], dataVBOIndex, elementVBOIndex, triangleNumber);
                }
            }

            /* Unmap buffer to send new data to the graphics card. If it returns false, the VBO data
             * must have got corruped, so throw an exception that is to be caught higher up the chain. */
            if (!glUnmapBuffer(GL_ARRAY_BUFFER))
            {
                throw debug::Exception(
                    "IndexedVBORenderer::Update - VBO data got corrupted when changing data.");
            }
            if (!glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER))
            {
                throw debug::Exception(
                    "IndexedVBORenderer::Update - VBO indices (elements) got corrupted when changing data.");
            }

            logger->WriteTextAndNewLine(logID, "IndexedVBORenderer successfully updated.");
        }



        void IndexedVBORenderer::RenderObject(IRenderable* renderable, unsigned int& index)
        {
            try
            {
                // If renderable has its own matrix, use it
                IMatrix* mat = dynamic_cast<IMatrix*>(renderable);
                if (mat)
                {
                    glPushMatrix(); // Stores current matrix

                    // Stores the matrix in the array format that OpenGL requires
                    float a[16];
                    mat->GetMatrixAsArray(a);

                    glMultMatrixf(a); // Multiplies matrix by the current one
                }


                // If it is textured, make sure that its texture is bound
                ISkinned* skinned = dynamic_cast<ISkinned*>(renderable);
                if (skinned)
                {
                    const std::string& skinID = skinned->GetSkinID(); // Gets skin ID from renderable

                    // Makes sure current skin isn't already active
                    if (renderDevice->GetCurrentSkinID() != skinID)
                    {
                        // Then makes the skin active
                        renderDevice->SetActiveSkin(skinID);
                    }
                }


                // If the renderable has INDEXED geometry, draw the arrays with the correct indexes!
                IIndexedGeometry* geometry = dynamic_cast<IIndexedGeometry*>(renderable);
                if (geometry)
                {
                    /* NOTE: Getting primitive type of the renderable is not needed here because
                     * all indexed geometry are assumed to be triangles. */

                    general::ArrayIndices indices = arrayIndices[index]; // Gets indices for this renderable

                    // Draws the vertices using the indices of the renderable's triangles
                    glDrawRangeElements(
                        GL_TRIANGLES, indices.start, (indices.start + indices.amount) - 1,
                        indices.amount, GL_UNSIGNED_INT, 0);

                }


                // If it's a group renderable, render all of its child objects
                IGroupRenderable* group = dynamic_cast<IGroupRenderable*>(renderable);
                if (group)
                {
                    for (unsigned int i = 0; (i < group->GetAmountOfRenderables()); i++)
                    {
                        if (group->GetRenderable(i) != NULL)
                        {
                            RenderObject(group->GetRenderable(i), index);
                        }
                    }
                }


                // After rendering, restore previous matrix if needed
                if (mat)
                {
                    glPopMatrix();
                }
            }
            catch (debug::Exception& ex)
            {
                ex.PrintMessage();
            }
            catch (std::exception& ex)
            {
                std::cout << ex.what() << std::endl;
            }
            catch (...)
            {
                std::cout
                    << "IndexedVBORenderer - Renderable# " << (index + 1) << " failed to render for unknown reasons!"
                    << std::endl;
            }

            // Increases the index of the arrayIndices
            index++;
        }


        void IndexedVBORenderer::Render()
        {
            // Bind the data and index VBOs
            glBindBuffer(GL_ARRAY_BUFFER, dataVBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);


            // Calculates offsets for array
            unsigned int vertexOffset = 0,
                texCoordOffset = sizeof(vector3f),
                normalOffset = sizeof(vector3f) + sizeof(vector2f);
            // Points to the vertex arrays in the VBO
            glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (GLvoid*)vertexOffset);
            glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (GLvoid*)texCoordOffset);
            glNormalPointer(GL_FLOAT, sizeof(Vertex), (GLvoid*)normalOffset);


            // Makes sure vertex arrays are enabled
            if (!glIsEnabled(GL_VERTEX_ARRAY)) glEnableClientState(GL_VERTEX_ARRAY);
            if (!glIsEnabled(GL_TEXTURE_COORD_ARRAY)) glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            if (!glIsEnabled(GL_NORMAL_ARRAY)) glEnableClientState(GL_NORMAL_ARRAY);

            // Used for accessing arrayIndices
            unsigned int index = 0;
            // Renders every object
            for (unsigned int i = 0; (i < renderables.size()); i++)
            {
                if (renderables[i] != NULL)
                {
                    RenderObject(renderables[i], index);
                }
            }


            // Unbinds the buffers and returns to client mode
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            logger->WriteTextAndNewLine(logID, "IndexedVBORenderer draws objects stored.");
        }

    }

}
