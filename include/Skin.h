/*
 * File:   Skin.h
 * Author: Donald
 *
 * Created on December 22, 2008, 8:44 PM
 */

#ifndef SKIN_H
#define SKIN_H

#include <string>

namespace parcel
{

namespace graphics
{

    // The maximum amount of textures a single skin can hold
    const unsigned int skinMaxTextures = 4;
    // Flag that determines whether a skin's textur slot is free
    const std::string skinNoTexture = "!";

    /* This struct holds the ID numbers of all all textures and the material is uses.
     * It also holds a boolean value that determines of the skin uses alpha values.
     * This can be used for optimisation; if it is false, then no processing time is
     * wasted on calculating alpha values for it. The static, constant variable
     * determines how many textures a single skin can use. noTexture is a
     * unique string that signifies that a slot in the skin does NOT have a texture.*/
    struct Skin
    {
        bool usesAlpha;
        std::string material;
        std::string textures[skinMaxTextures];

    };

}

}

#endif
