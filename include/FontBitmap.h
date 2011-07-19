/*
 * File:   FontBitmap.h
 * Author: Donald "Datriot" Whyte
 *
 * Created on May 5, 2009, 8:46 AM
 */

#ifndef FONTBITMAP_H
#define FONTBITMAP_H

#include <vector>
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

#include "Exceptions.h"

namespace parcel
{

namespace general
{

    /* Simple structure containing glyph data. */
    struct Glyph
    {
        int width, height; // Width and height of glyph
        int powWidth, powHeight; // Width and height to the nearest power-of-two integer
        float left; // Distance between center of glyph and its left-most pixel
        float advanceX; // How far the next glyph will be drawn from this one
        float underBase; // How many pixels glyph is underneath the baseline
        std::vector<unsigned char> bitmap; // Stores pixel data of glyph
    };

    /* This class is responsible for loading a TTF file and extracting
     * the bitmap and positioning data for every glyph (character) from it. */
    class FontBitmap
    {


    private:

        unsigned int height; // The height of the font
        std::vector<Glyph> glyphs; // Stores all the generated glyphs


        /* Loads an individual glyph and stores it the glyphs vector. */
        void LoadGlyph(const FT_Face& face, unsigned char character);


    public:

        /* Constructor, loads the font file and stores every glyph's
         * (from 0-128) bitmap. */
        FontBitmap(const std::string& filename, unsigned int fontHeight);
        ~FontBitmap();

        /* Creates a vertex array containing vertices and texture
         * coordinates for every glyph. Create array is returned. */
        std::vector<float> CreateVertexArray() const;

        /* Getters/setters. */
        unsigned int GetHeight() const
        {
            return height;
        }
        const Glyph& GetGlyph(unsigned char character) const
        {
            // Checks if glyph exists before returning
            if (character < glyphs.size()) return glyphs[character];
            // Otherwise, throw exception
            else throw debug::InvalidArgumentException("FontBitmap::GetGlyph - No glyph exists for this character!");
        }
        const std::vector<Glyph> GetGlyphs() const
        {
            return glyphs;
        }


    };

}

}

#endif
