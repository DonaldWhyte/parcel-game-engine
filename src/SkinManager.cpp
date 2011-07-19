/*
 * File:   SkinManager.cpp
 * Author: Donald "Datriot" Whyte
 *
 * Created on December 23, 2008, 7:23 PM
 * Modified to support new, string ID based storage of textures,
 * skins and materials on May 23, 2009, 10:16 AM
 */

#include <string>
#include <GLee.h>
#include <GL/glu.h>
#include "SkinManager.h"
#include "Exceptions.h"
#include "Logger.h"
#include "TGAImage.h"
#include "Colour.h"

namespace parcel
{

namespace graphics
{


    SkinManager::SkinManager(debug::Logger* log)
    {
        // Starts a new log for the skin manager and logs its creation
        logger = log;
        logID = logger->StartLog("SkinManager");
        logger->WriteTextAndNewLine(logID, "SkinManager created.");
    }
    SkinManager::~SkinManager()
    {
        DeleteAll(); // Releases resources used by the class

        logger->WriteTextAndNewLine(logID, "SkinManager destroyed.");
    }


    Skin* SkinManager::GetSkin(const std::string& id)
    {
        // Attempts to find skin
        SkinTable::iterator it = skins.find(id);
        // If it found it, returna pointer to it
        if (it != skins.end()) return &it->second;
        // Otherwise, throw an exception
        else throw debug::InvalidArgumentException("SkinManager::GetSkin - Cannot find skin with given ID.");
    }
    Material* SkinManager::GetMaterial(const std::string& id)
    {
        MaterialTable::iterator it = materials.find(id);

        if (it != materials.end()) return &it->second;
        else throw debug::InvalidArgumentException("SkinManager::GetMaterial - Cannot find material with given ID.");
    }
    Texture* SkinManager::GetTexture(const std::string& id)
    {
        TextureTable::iterator it = textures.find(id);

        if (it != textures.end()) return &it->second;
        else throw debug::InvalidArgumentException("SkinManager::GetTexture - Cannot find texture with given ID.");
    }
    const std::string& SkinManager::GetTextureName(const std::string& id) const
    {
        TextureTable::const_iterator it = textures.find(id);

        if (it != textures.end()) return it->second.name;
        else throw debug::InvalidArgumentException("SkinManager::GetTexture - Cannot find texture with given ID.");
    }



    bool SkinManager::AddSkin(const std::string& id, const Material& mat)
    {
        // Checks if material with given it already exists, throw exception if it does
        SkinTable::iterator it = skins.find(id);
        if (it != skins.end())
        {
            throw debug::InvalidArgumentException("SkinManager::AddSkin - Skin with ID '" +
                id + "' already exists!");
        }

        /* materialID is used to iterate thought all loaded materials and provide
         * the correct material ID for the skin. */
        unsigned int materialID = 0;
        bool newMaterial = true; // Flag to create a new material

        // Creates fresh new skin
        Skin newSkin;

        /* If there is a material identical to the one given that is already loaded,
         * use that instead. */
        for (MaterialTable::iterator itMat = materials.begin(); (itMat != materials.end()); itMat++)
        {
            /* If material is the same as the one just checked, give it the ID
             * of the material that already exists. */
            if (itMat->second == mat)
            {
                newSkin.material = materialID;
                newMaterial = false; // No need to create a new material
                break;
            }
        }
        // Otherwise, reallocate the material array and add the new material
        if (newMaterial)
        {
            // New material's ID will be skin's ID followed by "Material"
            std::string newMaterialID = id + "Material";
            // Adds the new material to the table
            materials[newMaterialID] = mat;
            newSkin.material = newMaterialID; // Gives new material's ID to the newly created skin
        }

        // Now, disables the skin's alpha usage and gives it default texture IDs
        newSkin.usesAlpha = false;
        for (unsigned int i = 0; (i < skinMaxTextures); i++)
            newSkin.textures[i] = skinNoTexture; // Uses an invalid texture index, since it does not use textures yet

        // Finally, adds the skin to the map
        skins[id] = newSkin;

        // Logs the event
        logger->WriteText(logID, "Skin " + id + " created.");
        logger->WriteTextAndNewLine(logID, " created.");

        return true; // Skin creation was a sucess, so return true!
    }
    bool SkinManager::AddSkin(const std::string& id, const colourf& ambient, const colourf& diffuse,
        const colourf& specular, const colourf& emmisive, const float& specularPower)
    {
        // Creates new material structure to hold material data
        Material mat =
        {
            ambient, diffuse, specular, emmisive,
            specularPower
        };

        return AddSkin(id, mat);
    }


    bool SkinManager::AddTexture(const std::string& textureID,
        const std::string& textureName, const float& textureTransparency,
        const TextureParameters& params)
    {
        // Creates the structure to hold the texture data in
        Texture texture;
        texture.name = textureName;
        texture.transparency = textureTransparency;

        // Calls function with appropriate parameters and returns result of it
        return AddTexture(textureID, texture, params);
    }
    bool SkinManager::AddTexture(const std::string& textureID, const Texture& texture,
        const TextureParameters& params)
    {
        // Checks if texture with this ID already exists
        TextureTable::iterator it = textures.find(textureID);
        if (it != textures.end())
        {
            throw debug::InvalidArgumentException("SkinManager::AddTexture - Texture with ID '" +
                textureID + "' already exists!");
        }
        // Checks if this texture is already loaded
        for (TextureTable::iterator itTex = textures.begin(); (itTex != textures.end()); itTex++)
        {
            // If it finds a texture with the same name
            if (itTex->second.name == texture.name)
            {
                // Return false to tell
                return false;
            }
        }

        // Loads new texture
        Texture newTexture = texture;

        // Loads the image by using the given name, also automatically resizes image to power-of-two
        TGAImage image;
        if (!image.LoadFromFile(newTexture.name, true))
            return false; // Returns flase if the image failed to load

        image.ConvertRGBToRGBA(255); // Makes sure image is in RGBA format

        // Assigns image data to texture structure, such as dimensions and pixel data of image
        newTexture.width = image.GetWidth();
        newTexture.height = image.GetHeight();
        newTexture.pixelData = image.GetPixelData();

        // Loads the texture using OPENGL and stores the OpenGL ID in the newly created texture object
        glGenTextures(1, &newTexture.glID);
        glBindTexture(GL_TEXTURE_2D, newTexture.glID);

        /* Uses image's pixel data to create the texture. If  mipmaps are enabled, creates
         * each mipmap level until a 1x1 texture is defined. */
        if (params.useMipmaps)
        {
            unsigned int mipmapLevel = 0; // Level of the current mipmap
            unsigned int width = image.GetWidth(); // Original width of texture
            unsigned int height = image.GetHeight(); // Original height of texture

            // Loops until width is lower than 1
            while(width >= 1)
            {
                glTexImage2D(GL_TEXTURE_2D, mipmapLevel, GL_RGBA, width,
                    height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.GetPixelData());

                // Divides the width and height by two, resulting in the dimensions of the next mipmap level
                width /= 2; height /= 2;
                // Increases mipmap level
                mipmapLevel += 1;
            }

            /* NOTE: does not work, need different pixel data for each mipmap level.  */

            // Uses TextureParameters struct, 'params', to set the texture's parameters
            SetTextureFiltering(params.minFilter, false);
            SetTextureFiltering(params.magFilter, true);
            SetTextureWrapping(params.sWrapping, 0);
            SetTextureWrapping(params.tWrapping, 1);
            SetTextureWrapping(params.rWrapping, 2);
        }
        // Otherwise, just create the first level with the original dimensions
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.GetWidth(),
                image.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.GetPixelData());

            // Uses TextureParameters struct, 'params', to set the texture's parameters
            SetTextureFiltering(params.minFilter, false);
            SetTextureFiltering(params.magFilter, true);
            SetTextureWrapping(params.sWrapping, 0);
            SetTextureWrapping(params.tWrapping, 1);
            SetTextureWrapping(params.rWrapping, 2);
        }

        // Stores created texture in the map using the correct ID
        textures[textureID] = newTexture;

        // Logs the event
        logger->WriteText(logID, "Texture " + textureID);
        logger->WriteTextAndNewLine(logID, " created.");
        return true; // The texture addition was a success!
    }


    bool SkinManager::AddTextureToSkin(const std::string& skinID, const std::string& textureID,
        const std::string& textureName, const float& textureTransparency, const bool& usesAlpha,
        const TextureParameters& params)
    {
        // Creates texture, initialising with given name and transparency value
        Texture texture;
        texture.name = textureName;
        texture.transparency = textureTransparency;

        // Calls function with appropriate parameters and returns result of it
        return AddTextureToSkin(skinID, textureID, texture, usesAlpha, params);
    }
    bool SkinManager::AddTextureToSkin(const std::string& skinID, const std::string& textureID,
        const Texture& texture, const bool& usesAlpha, const TextureParameters& params)
    {
        // Attemtps to find skin with given ID
        SkinTable::iterator it = skins.find(skinID);
        // If it failed to find the skin, just return false
        if (it == skins.end())
        {
            return false;
        }
        // used for cleaner code
        Skin& skin = it->second;

        // Checks if there are any slots free in the skin. If there isn't, return false.
        if (skin.textures[skinMaxTextures - 1] != skinNoTexture) return false;

        // Adds the texture and stores uses textureID as its ID
        AddTexture(textureID, texture, params);

        // Save the texture ID in the first free texture slot in the skin
        for (unsigned int i = 0; (i < skinMaxTextures); i++)
        {
            if (skin.textures[i] == skinNoTexture)
            {
                skin.textures[i] = textureID;
                break;
            }
        }

        // Logs the event
        logger->WriteTextAndNewLine(logID, "Texture " + textureID +
            " added to Skin " + skinID + ".");
        return true; // The texture addition was a success!
    }



    /* These simply call the four methods above, but passes a newly created,
     * default TextureParameters() struct.*/
    bool SkinManager::AddTexture(const std::string& textureID, const Texture& texture)
    { return AddTexture(textureID, texture, TextureParameters()); }

    bool SkinManager::AddTexture(const std::string& textureID, const std::string& textureName,
        const float& textureTransparency)
    { return AddTexture(textureID, textureName, textureTransparency, TextureParameters()); }

    bool SkinManager::AddTextureToSkin(const std::string& skinID, const std::string& textureID,
        const Texture& texture, const bool& usesAlpha)
    { return AddTextureToSkin(skinID, textureID, texture, usesAlpha, TextureParameters()); }

    bool SkinManager::AddTextureToSkin(const std::string& skinID, const std::string& textureID,
        const std::string& textureName, const float& textureTransparency, const bool& usesAlpha)
    {
        return AddTextureToSkin(skinID, textureID, textureName, textureTransparency,
            usesAlpha, TextureParameters());
    }


    bool SkinManager::DeleteSkin(const std::string& id)
    {
        // Attempts to find the skin
        SkinTable::iterator it = skins.find(id);
        // If found, delete it
        if (it != skins.end())
        {
            skins.erase(it);
            // Deletion was a success, log the event and return true
            logger->WriteTextAndNewLine(logID, "Material " + id + " deleted.");
            return true;
        }
        // If not found, return false
        else
        {
            return false;
        }
    }

    bool SkinManager::DeleteSkin(const std::string& id, bool deleteTextures, bool deleteMaterial)
    {
        // Attempts to find the skin
        SkinTable::iterator it = skins.find(id);
        // If found...
        if (it != skins.end())
        {
            // ...delete texture or material associated with the skin if the respective flags are set
            if (deleteTextures)
            {
                // Goes through the skin's textures, deleting every one
                for (unsigned int i = 0; (i < skinMaxTextures); i++)
                {
                    const std::string& texID = it->second.textures[i];
                    // If texture exists, delete it
                    if (texID != skinNoTexture)
                    {
                        DeleteTexture(texID);
                    }
                }
            }
            if (deleteMaterial)
            {
                DeleteMaterial(it->second.material);
            }
            // Then delete the skin
            skins.erase(it);

            logger->WriteTextAndNewLine(logID, "Skin " + id + " deleted.");
            return true;
        }
        // If not found, return false
        else
        {
            return false;
        }
    }

    bool SkinManager::DeleteTexture(const std::string& id)
    {
        /* NOTE: may make DeleteTexture() and DeleteMaterial() delete
         * any skin that uses the delete texture/material, but only
         * if the flag deleteSkin is set to true. */

        // Attempts to find the texture
        TextureTable::iterator it = textures.find(id);
        // If found...
        if (it != textures.end())
        {
            // Deletes the actual texture in the OpenGL driver
            glDeleteTextures(1, &it->second.glID);
            // Deletes texture object from the std::map
            textures.erase(it);

            logger->WriteTextAndNewLine(logID, "Texture " + id + " deleted.");
            return true;
        }
        // If not found, return false
        else
        {
            return false;
        }
    }

    bool SkinManager::DeleteMaterial(const std::string& id)
    {
        // Attempts to find the material
        MaterialTable::iterator it = materials.find(id);
        // If found...
        if (it != materials.end())
        {
            // Then delete the material
            materials.erase(it);

            logger->WriteTextAndNewLine(logID, "Material " + id + " deleted.");
            return true;
        }
        // If not found, return false
        else
        {
            return false;
        }
    }

    void SkinManager::DeleteAll()
    {
        // Deletes every texture that is currently loaded in OPENGL
        for (TextureTable::iterator it = textures.begin(); (it != textures.end()); it++)
        {
            // Checks if the texture exists before trying to delete it
            if (glIsTexture(it->second.glID))
            {
                glDeleteTextures(1, &it->second.glID);
            }
        }
        // Clears the skin, texture and material tables
        skins.clear();
        textures.clear();
        materials.clear();
        // Logs the event
        logger->WriteTextAndNewLine(logID, "SkinManager's contents has been deleted. All skins, textures and materials.");
    }

    void SkinManager::SetTextureFiltering(TextureFilter filter, bool magnification)
    {
        // Gets what filter mode it applies to by using the given boolean
        GLenum mag = (magnification) ? GL_TEXTURE_MAG_FILTER : GL_TEXTURE_MIN_FILTER;
        // Changes filtering settings of the filtering mode decided above
        switch (filter)
        {
            case TEXTUREFILTER_LINEAR:
                glTexParameterf(GL_TEXTURE_2D, mag, GL_LINEAR); break;
            case TEXTUREFILTER_NEAREST:
                glTexParameterf(GL_TEXTURE_2D, mag, GL_NEAREST); break;
            case TEXTUREFILTER_LINEARLINEAR:
                glTexParameterf(GL_TEXTURE_2D, mag, GL_LINEAR_MIPMAP_LINEAR); break;
            case TEXTUREFILTER_LINEARNEAREST:
                glTexParameterf(GL_TEXTURE_2D, mag, GL_LINEAR_MIPMAP_NEAREST); break;
            case TEXTUREFILTER_NEARESTLINEAR:
                glTexParameterf(GL_TEXTURE_2D, mag, GL_NEAREST_MIPMAP_LINEAR); break;
            case TEXTUREFILTER_NEARESTNEAREST:
                glTexParameterf(GL_TEXTURE_2D, mag, GL_NEAREST_MIPMAP_NEAREST); break;
            default:
                throw debug::InvalidArgumentException(
                    "SkinManager::SetTextureFiltering - Invalid minification filter specified.");
        }
    }

    void SkinManager::SetTextureWrapping(TextureWrapping wrapping, unsigned int textureCoordinate)
    {
        // Gets which coordinate this wrapping mode applies to by using the given integer
        GLenum coordinate = 0;
        switch(textureCoordinate)
        {
            case 0: coordinate = GL_TEXTURE_WRAP_S; break;
            case 1: coordinate = GL_TEXTURE_WRAP_T; break;
            case 2: coordinate = GL_TEXTURE_WRAP_R; break;
            // Throws exception if the coordinate is none of the above
            default:
                throw debug::InvalidArgumentException(
                    "SkinManager::SetTextureWrapping - Invalid wrapping coordinate specified.");
        }
        // Sets the appropriate wrapping mode for the coordinate of this texture
        switch(wrapping)
        {
            case TEXTUREWRAP_CLAMP:
                glTexParameterf(GL_TEXTURE_2D, coordinate, GL_CLAMP); break;
            case TEXTUREWRAP_CLAMPTOEDGE:
                glTexParameterf(GL_TEXTURE_2D, coordinate, GL_CLAMP_TO_EDGE); break;
            case TEXTUREWRAP_REPEAT:
                glTexParameterf(GL_TEXTURE_2D, coordinate, GL_REPEAT); break;
            default:
                throw debug::InvalidArgumentException(
                    "SkinManager::SetTextureWrapping - Invalid wrapping option specified.");
        }
    }

}

}
