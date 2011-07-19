/*
 * File:   AModelLoader.h
 * Author: Donald "Datriot" Whyte
 *
 * Created on March 21, 2009, 9:49 AM
 */

#ifndef AMODELLOADER_H
#define AMODELLOADER_H

#include <vector>
#include <string>
#include "Vertex.h"
#include "Primitives.h"

namespace parcel
{

namespace graphics
{

    /* Abstract class. All other model loaders derive from this. */
    class AModelLoader
    {


    protected:


        std::vector<Vertex> vertices; // Stores the full vertex, with its position, tex coord and normal
        std::vector<Triangle> triangles; // Stores triangle data which are indexes into the vertex data

        std::vector<std::string> loadedSkins; // Stores IDs to all the skins loaded using SkinManager


    public:


        /* Blank constructor and destructor. */
        AModelLoader()
        {
        }

        virtual ~AModelLoader()
        {
        }


        /* Abstract method to load a model using the given filename. Returns false if file failed to load.
         * Needs to be implemented by subclasses. */
        virtual bool LoadFromFile(const std::string& filename) = 0;


        /* Getters, used to acquire the model's vertices, triangles and skins. */
        virtual const std::vector<Vertex>& GetVertices() { return vertices; }
        virtual const std::vector<Triangle>& GetTriangles() { return triangles; }

        virtual const std::vector<std::string>& GetSkins() { return loadedSkins; }
        // TODO: add animation/bone getter here???


    };

}

}

#endif
