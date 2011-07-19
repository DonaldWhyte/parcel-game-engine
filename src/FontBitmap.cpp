/*
 * File:   FontBitmap.cpp
 * Author: Donald "Datriot" Whyte
 *
 * Created on May 5, 2009, 8:47 AM
 */

#include <string>
#include "FontBitmap.h"
#include "MCommon.h"

namespace parcel
{

namespace general
{

    FontBitmap::FontBitmap(const std::string& filename, unsigned int fontHeight) :
        height(fontHeight)
    {
        // Creates a library to hold the font
        FT_Library library;
        // Throws an exception if the library initialisation fails
        if (FT_Init_FreeType(&library))
        {
            throw debug::Exception("FontBitmap::FontBitmap - Failed to create library to store font!");
        }
        // Now loads the font into the library
        FT_Face face;
        // Exception is thrown if the font fails to load
        if (FT_New_Face(library, filename.c_str(), 0, &face))
        {
            throw debug::Exception("FontBitmap::FontBitmap - Cannot load font " + filename + "!");
        }
        /* Sets the size of the loaded font, using the fontHeight argument.
         * NOTE: "<< 6" used to make sure the given size is divisible by 64,
         * which is a requirement for FreeType2 to work. */
        FT_Set_Char_Size(face, height << 6, height << 6, 96, 96);

        // Loads the neccessery data for every character
        for (unsigned char i = 0; (i < 128); i++)
        {
            LoadGlyph(face, i);
        }
    }

    FontBitmap::~FontBitmap()
    {
    }

    void FontBitmap::LoadGlyph(const FT_Face& face, unsigned char character)
    {
        // Attempts to load the glyph that represents the given character
        if (FT_Load_Glyph(face, FT_Get_Char_Index(face, character), FT_LOAD_DEFAULT))
        {
            std::string str = "FontBitmap::LoadBitmap - Could not load character " + character;
            str += "!";
            throw debug::Exception(str);
        }
        // Gets the loaded glyph
        FT_Glyph glyph;
        if (FT_Get_Glyph(face->glyph, &glyph))
        {
            std::string str =  "FontBitmap::LoadBitmap - Could not retrieve glyph " + character;
            str += "!";
            throw debug::Exception(str);
        }
        // Converts the glyph to a bitmap object
        FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
        FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph)glyph;
        // Makes accesssing the bitmap easier
        FT_Bitmap bitmap = bitmapGlyph->bitmap;

        /* Converts width and height of bitmap to a power-of-two size, so it
         * works better with textures. */
        int width = maths::ClosestPow2(bitmap.width);
        int height = maths::ClosestPow2(bitmap.rows);

        /* Creates an std::vector that holds the 2-channel (luminance) data
         * that the FreeType2 fonts produce. Size of vector is determined
         * by the dimensions of the bitmap produced by the font. */
        std::vector<unsigned char> pixels(2 * width * height);
        // Fills in the pixels vector with the bitmap data acquired from the font
        for (int i = 0; (i < height); i++)
        {
            for (int j = 0; (j < width); j++)
            {
                pixels[2 * (j + i * width)] = pixels[2 * (j + i * width) + 1] =
                    (j >= bitmap.width || i >= bitmap.rows) ?
                    0 : bitmap.buffer[j + bitmap.width * i];
            }
        }


        // Creates a glyph that contains the character's bitmap among other things
        Glyph generatedGlyph = {
            bitmap.width, // width
            bitmap.rows, // height
            maths::ClosestPow2(bitmap.width), // nearest power-of-two width
            maths::ClosestPow2(bitmap.rows), // nearest power-of-two height
            bitmapGlyph->left, // how many pixels left glyph needs to be drawn
            face->glyph->advance.x >> 6, // how far to position next character
            (bitmap.rows - bitmapGlyph->top-bitmap.rows), // Used for baseline positioning
            pixels // Bitmap data
            };
        // Adds the array of pixel data to the bitmaps vector
        glyphs.push_back(generatedGlyph);

        // Cleans up glyph data
        FT_Done_Glyph(glyph);
    }

    std::vector<float> FontBitmap::CreateVertexArray() const
    {
        std::vector<float> array;

        // Creates the vertices and texture coordinates for every glyph
        for (unsigned char i = 0; (i < glyphs.size()); i++)
        {
            // Gets the glyph's dimensions and their power-of-two equivilents
            int width = glyphs[i].width, height = glyphs[i].height;
            int powWidth = glyphs[i].powWidth, powHeight = glyphs[i].powHeight;
            // Calculates glyph's texture coordinates
            float texWidth = (float)width / (float)powWidth;
            float texHeight = (float)height / (float)powHeight;
            // Creates and adds the vertices and texture coordinates to the array
            array.push_back(0); array.push_back(0); // vertex1
            array.push_back(0); array.push_back(0); // tcoord1
            array.push_back(0); array.push_back(height); // vertex2
            array.push_back(0); array.push_back(texHeight); // tcoord2
            array.push_back(width); array.push_back(height); // vertex3
            array.push_back(texWidth); array.push_back(texHeight); // tcoord3
            array.push_back(width); array.push_back(0); // vertex4
            array.push_back(texWidth); array.push_back(0); // tcoord4
        }

        // Returns the filled array
        return array;
    }

}

}
