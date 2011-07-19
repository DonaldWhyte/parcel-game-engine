/*
 * File:   DMFModelLoader.h
 * Author: Donald "Datriot" Whyte
 *
 * Created on March 21, 2009, 9:50 AM
 */

#ifndef DMFMODELLOADER_H
#define DMFMODELLOADER_H

#include "AModelLoader.h"
#include "Material.h"
#include "SkinManager.h"
#include "Animation.h"

namespace parcel
{

namespace graphics
{


    /* Holds all the enumerators, structures, methods and variables to read
     * and write 3D models stored as a DMF (Datriot Model File). */
    class DMFModelLoader : public AModelLoader
    {


    private:

        /* Contains all the identifiers which may be present in a DMF file. */
        enum DMFIdentifiers
        {
            DMF_FILE = 0x000F,

            DMF_GEOMETRY_SPEC = 0x0002,
            DMF_VERTICES = 0x0012,
            DMF_TRIANGLES = 0x0022,

            DMF_SKIN_SPEC = 0x0003,
            DMF_TEXTURES = 0x0013,
            DMF_MATERIALS = 0x0023,
            DMF_SKINS = 0x0033,

            DMF_ANIMATION_SPEC = 0x0004,
//            DMF_BONE = 0x0014,
//            DMF_ANIMATION = 0x0024,
//            DMF_KEYFRAME = 0x0034,

            DMF_END = 0x00FF
        };


        /* Struct containing header of the model format. The headers holds vital
         * information about the file, such as how many textures, materials or
         * vertices there are and the version of the format. */
        struct DMFHeader
        {
            int version; // Contains version of DMF format being used

            /* Holds the amount of each component of the model, used to
             * read the correct amount of memory in the file. */
            int numVertices;
            int numTriangles;

            int numSkins;
            int numTextures;
            int numMaterials;

//            int numBones;
//            int numAnimations;
        };


        /* Structures used for holding the skin, texutre and material data that are used
         * for DMF_SKINS, DMF_TEXTURES AND DMF_MATERIALS respectively. Each hold
         * an ID and any data specific to their item. */
        struct DMFSkin
        {
            unsigned int id;
            unsigned int textureID, materialID; // IDs of texture and material used
        };

        struct DMFTexture
        {
            unsigned int id;
            std::string filename; // Filename of the texture
        };

        struct DMFMaterial
        {
            unsigned int id;
            Material material; // Holds actual values for material
        };


        /* Holds the skins, textures and material data loaded from a DMF model. */
        std::vector<DMFSkin> skins;
        std::vector<DMFTexture> textures;
        std::vector<DMFMaterial> materials;

        /* TODO. */
        std::vector<Bone> bones;
        std::vector<Animation> animations;

        SkinManager* skinManager; // Used for loading the actual skin and textures for the engine


    public:

        /* Default constructor and destructor. The constructor takes the game's SkinManager to
         * load all the skins into the Parcel engine. */
        DMFModelLoader(SkinManager* sManager);
        ~DMFModelLoader();

        /* Loads the DMF file format and stores the data retrieved in the appropriate vectors. */
        bool LoadFromFile(const std::string& filename);

        /* Writes the given model to a file. The filename of the model is given in the parameters. */
        static bool WriteToFile(const DMFModelLoader& model, const std::string& filename);


    };

}

}

#endif
