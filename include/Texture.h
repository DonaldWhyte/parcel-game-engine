/*
 * File:   Texture.h
 * Author: Donald "Datriot" Whyte
 *
 * Created on December 23, 2008, 7:32 PM
 * Updated to add TextureParameters struct and related enumerators
 * on July 1 11:10 AM
 */

#ifndef TEXTURE_H
#define TEXTURE_H

#include <GLee.h>
#include <string>
#include "Colour.h"

namespace parcel
{

namespace graphics
{

    /* Contains the information for a basic texture. This includes the transparency of
     * the texture, the filename of the image it got the pixel data from, a pointer to
     * the actual pixel data and the dimensions of the source image. */
    struct Texture
    {
        std::string name; // Texture's filename
        int width, height; // Dimensions of source image

        float transparency; // Transparency of texture
        const void* pixelData; // The actual pixel data that makes up the texture

        GLuint glID; // The ID used for texture that is loaded using OPENGL
    };

    /* Enumerators for texture filtering and wrapping. Used for TextureParameters. */
    enum TextureFilter
    {
        TEXTUREFILTER_LINEAR,
        TEXTUREFILTER_NEAREST,
        // These four can only be used if mipmaps are being used
        TEXTUREFILTER_LINEARLINEAR,
        TEXTUREFILTER_LINEARNEAREST,
        TEXTUREFILTER_NEARESTLINEAR,
        TEXTUREFILTER_NEARESTNEAREST
    };
    enum TextureWrapping
    {
        TEXTUREWRAP_CLAMP,
        TEXTUREWRAP_CLAMPTOEDGE,
        TEXTUREWRAP_REPEAT
    };

    /* This structure is passed to the SkinManager when it loads a texture.
     * It determines how the texture is renderer, how it reacts when it is
     * minimised and magnified and so on. */
    struct TextureParameters
    {

        bool useMipmaps; // If true, SkinManager loads mipmaptextures of this one

        // Determines which texels are chosen when the texture is minimised and magnified
        TextureFilter minFilter, magFilter;
        // Determines how the texture wraps around the geometry
        TextureWrapping sWrapping, tWrapping, rWrapping;

        // The colour of teh texture's border, shown on some filtering and wrapping modes
        colourf borderColour;
        /* The texture's priority in memory, must be between 0 and 1. The higher the number,
         * the longer it will stay in the grahpic card's memory when it hasn't been used
         * for a while or there are too many textures for the card to hold. */
        float priority;

        /* Default constructor. Initialises struct's members with a default setting. */
        TextureParameters() :
            useMipmaps(false), minFilter(TEXTUREFILTER_NEAREST), magFilter(TEXTUREFILTER_NEAREST),
            sWrapping(TEXTUREWRAP_CLAMP), tWrapping(TEXTUREWRAP_CLAMP), rWrapping(TEXTUREWRAP_CLAMP),
            borderColour(presetcolours::Black), priority(1.0f)
        {
        }
        /* This constructor initialises the struct's members using the given values. */
        TextureParameters(bool tUseMipmaps, TextureFilter tMinFilter, TextureFilter tMagFilter,
            TextureWrapping tSWrapping, TextureWrapping tTWrapping, TextureWrapping tRWrapping,
            const colourf& tBorderColour, float tPriority) :
            useMipmaps(tUseMipmaps), minFilter(tMinFilter), magFilter(tMagFilter),
            sWrapping(tSWrapping), tWrapping(tTWrapping), rWrapping(tRWrapping),
            borderColour(tBorderColour), priority(tPriority)
        {
        }

    };

}

}

#endif
