/*
 * File:   RenderInterfaces.h
 * Author: Donald "Datriot" Whyte
 *
 * Created on February 17, 2009, 9:44 AM
 */

#ifndef RENDERINTERFACES_H
#define RENDERINTERFACES_H

#include <vector>
#include <string>
#include "Vertex.h"
#include "Primitives.h"
#include "Vector.h"
#include "Matrix.h"

namespace parcel
{

    /* A collection of interfaces that are used in ARenderer/VBORenderer.
     * NOTE: I may add a few macros in a CppInterface.h that allows me to use
     * the 'interface' keyword to declare interfaces and 'implements' to make classes
     * inherit the interface.  */
    namespace graphics
    {


        /* Main interface that determines if an object can be rendered and used in a renderer.
         *
         * GetMemorySize() is used for any object that contains geometry and uses the
         * IGeometry interface. It doesn't return the memory size for all its vertices, tex coords
         * and normals, but just the vertices, since they all must have the amount. */
        class IRenderable
        {

        public:

            virtual int GetMemorySize() = 0;

        };


        /* Group nodes or renderables that are dependant on the status of the object should
         * inherit this and add the renderables to this instead of the renderer.
         * NOTE: The GetMemorySize() method in IRenderable MUST represent the memory
         * size of all the renderables stored in a GroupRenderable.
         *
         * GetAmountOfRenderables() returns the amount of IRenderable objects stored in
         * this group.

         * GetRenderable() returns a pointer to a renderable with the index specified. */
        class IGroupRenderable
        {

        public:

            virtual unsigned int GetAmountOfRenderables() = 0;

            virtual IRenderable* GetRenderable(unsigned int i) = 0;

        };


        /* Holds the position and orientation of a renderable object, relative it its parents.
         *
         * GetMatrix() returns the matrix used to transform the renderable implementing
         * this interface.
         *
         * GetMatrixAsArray() takes the given array (it assumes it has enough space for
         * all the elements) and fills it with the matrix's elements, in column-major order.
         *
         * SetMatrix() gives the renderable a new matrix (which can change its position,
         * orientation and size). */
        class IMatrix
        {

        public:

            virtual void GetMatrixAsArray(float* a) = 0;

            virtual const maths::matrixf& GetMatrix() = 0;

        };


        /* Simply used to identify is renderable has geometry to be rendered.
         *
         * GetVertices() returns an std::vector of Vertex structs. The struct holds
         * three floats for position, three floats for normal and two floats for texture coordinates.
         *
         * GetPrimitiveType() returns an enumeration deciding which primitive type to use; tris,
         * quads, strips, fans and so on. */
        class IGeometry
        {

        public:

            virtual const std::vector<Vertex>& GetVertices() = 0;

            virtual int GetPrimitiveType() = 0;

        };


        /* Used for renderables with textures and materials by storing an ID to their skin.
         *
         * GetSkinID() returns the ID of the skin (in the RenderDevice's SkinManager instance). */
        class ISkinned
        {

        public:

            virtual const std::string& GetSkinID() = 0;

        };


        /* Used for renderables that contain vertex data but uses triangle lists
         * (indices) for drawing those vertices, such as models. Triangles are
         * always the assumed primitive type when using this interface, so
         * no GetPrimitiveType() is required. NOTE: A renderable CANNOT
         * inherit from both IGeometry and IIndexedGeometry, it is bad practice
         * and may cause undefined errors.
         *
         * GetVertices() is pretty much the same as IGeometry's version.
         *
         * GetFaces() returns an std::vector of Triangle structs. The struct holds
         * three integers for the vertex indices, three integers for normal indices
         * and two integers for texture coordinate indices.
         *
         * GetTriangleMemorySize() returns the memory size of all the indices
         * that make up every triangle. It is the equivilent of GetMemorySize(),
         * but for the index (element) renderer. */
        class IIndexedGeometry
        {

        public:

            virtual const std::vector<Vertex>& GetVertices() = 0;

            virtual const std::vector<Triangle>& GetFaces() = 0;

            virtual int GetTriangleMemorySize() = 0;

        };


        /* Used for renderables that are flat and used as 2D images. This can only
         * be used with SpriteRenderer, other renderers will simply ignore it.
         *
         * GetVertices() is the same as IGeometry, except it returns SpriteVertex
         * objects, which do not have a normal and only 2 floats for a position. */
        class ISprite
        {

        public:

            virtual const std::vector<SpriteVertex>& GetVertices() = 0;

        };


    }

}


#endif
