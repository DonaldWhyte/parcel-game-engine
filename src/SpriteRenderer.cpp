/*
 * File:   SpriteRenderer.cpp
 * Author: Donald "Datriot" Whyte
 *
 * Created on June 29, 2009, 7:05 PM
 */

#include "SpriteRenderer.h"
#include "Vertex.h"
#include "Primitives.h"
#include "Exceptions.h"

namespace parcel
{

using namespace maths;

namespace graphics
{

    SpriteRenderer::SpriteRenderer(RenderDevice* renderDevice, debug::Logger* log, const bool& willDeleteAll) :
        ARenderer(log, "SpriteRenderer", willDeleteAll), // Calls superclass' constructor
        vboID(0), vboData(NULL), renderDevice(renderDevice)
    {
        // Stores currently bound array buffer
        int arrBuffer;
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &arrBuffer);
        // Generates this renderer's VBO
        glGenBuffers(1, &vboID);
        glBindBuffer(GL_ARRAY_BUFFER, vboID);
        glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);
        // Restores the array buffer that was active BEFORE the creation of this renderer
        glBindBuffer(GL_ARRAY_BUFFER, arrBuffer);

        logger->WriteTextAndNewLine(logID, "SpriteRenderer created.");
    }

    SpriteRenderer::~SpriteRenderer()
    {
        // Deletes VBO
        glDeleteBuffers(1, &vboID);
        logger->WriteTextAndNewLine(logID, "SpriteRenderer destroyed.");
    }

    void SpriteRenderer::ProcessRenderable(IRenderable* renderable, unsigned int& vboIndex, unsigned int& vertexNumber)
    {
        // Stores start and end of this renderable's vertices in the VBO array
        general::ArrayIndices indices;
        // Sets the start of the vertices
        indices.start = vertexNumber;

        unsigned int i; // Iterator for the loops
        unsigned int amount = 0; // The amount of vertices this renderable has to draw

        // If renderable holds geometry
        ISprite* sprite = dynamic_cast<ISprite*>(renderable);
        if (sprite)
        {
            // Gets the vertices from the renderable and fills the VBO with them
            const std::vector<SpriteVertex>& vertexData = sprite->GetVertices();
            for (i = 0; (i < vertexData.size()); i++)
            {
                // Stores vertex position and texture cooridnates
                vboData[vboIndex++] = vertexData[i].position.x;
                vboData[vboIndex++] = vertexData[i].position.y;
                vboData[vboIndex++] = vertexData[i].texCoord.x;
                vboData[vboIndex++] = vertexData[i].texCoord.y;
                // Increases the amount of vertices, so it draws this vertex too
                amount++;
                // Also increases the total vertex number
                vertexNumber++;
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
                    ProcessRenderable(group->GetRenderable(i), vboIndex, vertexNumber);
                }
            }
        }

        // Sets amount of vertices for indices and pushes the indices into the vector
        indices.amount = amount;
        arrayIndices.push_back(indices);
    }

    void SpriteRenderer::Update()
    {
        // Gets the size the updated buffer will need to be
        vboMemorySize = 0; // Resets memory size to 0
        // Iterates through all renderables, adding their memory size to the total
        for (unsigned int i = 0; (i < renderables.size()); i++)
        {
            vboMemorySize += renderables[i]->GetMemorySize();
        }

        // Binds the renderer's buffer to make it active
        glBindBuffer(GL_ARRAY_BUFFER, vboID);
        // Clears VBO and specifies how the data in the arrays will be packed
        glBufferData(GL_ARRAY_BUFFER, vboMemorySize, NULL, GL_DYNAMIC_DRAW);

        // If memory size is zero, just return since there is nothing to update
        if (vboMemorySize == 0) return;

        // Gets pointer to VBO data to put our values into
        vboData = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

        // Used for accessing VBO array's elements
        unsigned int vboIndex = 0;
        // Used for getting the start index for each renderable's ArrayIndices struct
        unsigned int vertexNumber = 0;
        // Clears the std::vector of its array indices and reserves needed amount of memory
        arrayIndices.clear();
        arrayIndices.reserve(renderables.size());

        /* Processes all the renderables, adding data to the VBO and putting indices to that
         * data into arrayIndices. */
        for (unsigned int i = 0; (i < renderables.size()); i++)
        {
            if (renderables[i] != NULL)
            {
                ProcessRenderable(renderables[i], vboIndex, vertexNumber);
            }
        }

        /* Unmap buffer to send new data to the graphics card. If it returns false, the VBO data
         * must have got corruped, so throw an exception that is to be caught higher up the chain. */
        if (!glUnmapBuffer(GL_ARRAY_BUFFER))
        {
            throw debug::Exception("SpriteRenderer::Update - VBO data got corrupted when changing data.");
        }

        logger->WriteTextAndNewLine(logID, "SpriteRenderer successfully updated.");
    }

    void SpriteRenderer::RenderObject(IRenderable* renderable, unsigned int& index)
    {
        try
        {
            // Just return if there is no gemoetry to draw
            if (vboMemorySize <= 0) return;

            // If renderable has its own matrix, use it
            IMatrix* mat = dynamic_cast<IMatrix*>(renderable);
            if (mat)
            {
                glPushMatrix();

                float a[16];
                mat->GetMatrixAsArray(a);

                glMultMatrixf(a);
            }

            // If it is textured, make sure that its texture is bound
            ISkinned* skinned = dynamic_cast<ISkinned*>(renderable);
            if (skinned)
            {
                const std::string& skinID = skinned->GetSkinID();
                if (renderDevice->GetCurrentSkinID() != skinID)
                {
                    renderDevice->SetActiveSkin(skinID);
                }
            }

            // If the renderable has geometry, draw the arrays with the correct indexes
            ISprite* sprite = dynamic_cast<ISprite*>(renderable);
            if (sprite)
            {
                glDrawArrays(GL_QUADS, arrayIndices[index].start, arrayIndices[index].amount);
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
                << "SpriteRenderer - Renderable " << (index + 1) << " failed to render for unknown reasons!"
                << std::endl;
        }


        // Increases the index of the arrayIndices
        index++;
    }

    void SpriteRenderer::Render()
    {
        // Bind the vertex buffer object
        glBindBuffer(GL_ARRAY_BUFFER, vboID);

        // Calculates offsets for array
        unsigned int vertexOffset = 0, texCoordOffset = sizeof(vector2f);
        // Points to the vertex arrays in the VBO
        glVertexPointer(2, GL_FLOAT, sizeof(SpriteVertex), (GLvoid*)vertexOffset);
        glTexCoordPointer(2, GL_FLOAT, sizeof(SpriteVertex), (GLvoid*)texCoordOffset);
        // Enables vertex arrays
        if (!glIsEnabled(GL_VERTEX_ARRAY)) glEnableClientState(GL_VERTEX_ARRAY);
        if (!glIsEnabled(GL_TEXTURE_COORD_ARRAY)) glEnableClientState(GL_TEXTURE_COORD_ARRAY);
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

        // Unbinds the buffer and returns to client mode
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        logger->WriteTextAndNewLine(logID, "SpriteRenderer draws objects stored.");
    }

}

}
