/*
 * File:   SkinManager.h
 * Author: Donald "Datriot" Whyte
 *
 * Created on December 23, 2008, 7:05 PM
 * Modified to support new, string ID based storage of textures,
 * skins and materials on May 23, 2009, 10:16 AM
 */

#ifndef SKINMANAGER_H
#define SKINMANAGER_H

#include <map>
#include <string>
#include "Material.h"
#include "Texture.h"
#include "Skin.h"
#include "Logger.h"

namespace parcel
{

namespace graphics
{

    /* Creates, manages and deletes all skins, materials and textures used in the game. */
    class SkinManager
    {


    private:

        // These constants define the maximum amount this manager can hold
        static const unsigned int maxTextures = 20;
        // Maps are typedef'd for shorter code
        typedef std::map<std::string, Texture> TextureTable;
        typedef std::map<std::string, Skin> SkinTable;
        typedef std::map<std::string, Material> MaterialTable;

        /* These tables hold every texture, skin and material along with their respective
         * string ID. */
        TextureTable textures;
        SkinTable skins;
        MaterialTable materials;

        debug::Logger* logger; // Pointer to the logger being used in the game
        unsigned int logID; // ID of the log created for the skin manager


        /* Sets the filtering parameters for a single texture. Called in AddTexture(). */
        void SetTextureFiltering(TextureFilter filter, bool magnification);
        /* Sets the wrapping parameters for a coordinate of a single texture. Like
         * SetTextureFiltering(), this is called in AddTexture(). */
        void SetTextureWrapping(TextureWrapping wrapping, unsigned int textureCoordinate);


    public:

        SkinManager(debug::Logger* log);
        virtual ~SkinManager(); // Destructor

        void DeleteAll(); // Deletes all skins/materials/textures currently loaded

        Skin* GetSkin(const std::string& id); // Returns a pointer to the skin with the given ID
        Material* GetMaterial(const std::string& id); // Same as above, but for materials
        Texture* GetTexture(const std::string& id); // Same as above, but for textures
        // Returns the filename of the texture with the given ID
        const std::string& GetTextureName(const std::string& id) const;

        /* Adds a skin and use the material given as the new skin's material. The string
         * given will be the skin's new ID, unless that ID already exists, which it will
         * return false and NOT create the skin. If the given material is equal to another
         * material already loaded, it will use the material already loaded and ignore the
         * given material to save memory. */
        bool AddSkin(const std::string& id, const Material& mat);
        /* Overload so we don't have to create a new material structure. */
        bool AddSkin(const std::string& id, const colourf& ambient,
            const colourf& diffuse, const colourf& specular, const colourf& emmisive,
            const float& specularPower);


        /* Adds a texture but does not add it to a skin, simply creates one and stores it. */
        bool AddTexture(const std::string& textureID, const Texture& texture,
            const TextureParameters& params);
        /* This overload turns parameters into a Texture struct and calls the above method. */
        bool AddTexture(const std::string& textureID, const std::string& textureName,
            const float& textureTransparency, const TextureParameters& params);

        /* Adds a texture to a skin (by using skinID to find the needed skin).
         * This also checks if the given texture has already been created; if it has,
         * then simply use the texture that is already created for the skin's texture. */
        bool AddTextureToSkin(const std::string& skinID, const std::string& textureID,
            const Texture& texture, const bool& usesAlpha, const TextureParameters& params);
        // Overload, translates texture data into a struct for you and then calls the above. */
        bool AddTextureToSkin(const std::string& skinID, const std::string& textureID,
            const std::string& textureName, const float& textureTransparency, const bool& usesAlpha,
            const TextureParameters& params);

        /* Overloads of the four methods above that don't need to be give a
         * TextureParameters struct, a default one is created automatically.
         * This means I don't have to create an extra TextureParameters()
         * structure myself when I don't need to, shortening code. */
        bool AddTexture(const std::string& textureID, const Texture& texture);
        bool AddTexture(const std::string& textureID, const std::string& textureName,
            const float& textureTransparency);

        bool AddTextureToSkin(const std::string& skinID, const std::string& textureID,
            const Texture& texture, const bool& usesAlpha);
        bool AddTextureToSkin(const std::string& skinID, const std::string& textureID,
            const std::string& textureName, const float& textureTransparency, const bool& usesAlpha);

        /* Finds skin using the given ID and deletes it. The second overload, with
         * the two boolean parameters determines if the texture and material used
         * by the skin are delete too. Be very careful when setting these to true. */
        bool DeleteSkin(const std::string& id);
        bool DeleteSkin(const std::string& id, bool deleteTextures, bool deleteMaterial);
        /* These both find the texture/material using the given ID and deletes it.
         * If a skin currently uses the texture or material in question, that skin
         * will also be delete, so BEWARE! */
        bool DeleteTexture(const std::string& id);
        bool DeleteMaterial(const std::string& id);


    };

}

}

#endif
