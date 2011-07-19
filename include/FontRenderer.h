/*
 * File:   FontRenderer.h
 * Author: Donald "Datriot" Whyte
 *
 * Created on May 5, 2009, 9:17 AM
 */

#ifndef FONTRENDERER_H
#define FONTRENDERER_H

#include <vector>
#include "FontBitmap.h"
#include "Colour.h"
#include "Vector.h"

namespace parcel
{

namespace graphics
{

    /* This class is repsonsible for creating AND drawing the font using textured quads. */
    class FontRenderer
    {


    private:

        std::vector<general::Glyph> glyphs; // Stores all the glyph for use later when drawing
        unsigned int numGlyphs; // Amount of glyphs in the font
        float height; // Height of the font we want to draw

        float hMargin; // Determines the starting X coordinate of a new line
        float vMargin; // Determines the starting Y coordinate of the next drawn line
        float lineSpacing; // Determines extra space inbetween each line of text
        bool freshLine; // If freshLine is true, then the next piece of text will be drawn at the hMargin

        maths::vector2f offset; // Determines where each character will be drawn
        colourf textColour; // RGBA colour of the drawn text

        GLuint textures[128]; // Stores texture IDs for each glyph
        GLuint displayLists; // Stores base display list, used for accessing every glyph's list

        /* Creates the textures and display lists to render later.
         * Called in the constructor. */
        void Initialise(const general::FontBitmap& fontBitmap);

        /* Loads the given glyph's texture into the driver. */
        void LoadTexture(const general::Glyph& glyph, unsigned char character);
        /* Builds  the geometry data and display list for the given glyph. */
        void BuildDisplayList(const general::Glyph& glyph, unsigned char character);
        /* Splits the given string into many strings for every line and returns them. */
        std::vector<std::string> FontRenderer::ParseLines(const std::string& text);

        /* Draws the stirng using the loaded font. */
        void Draw(const std::string& text);


    public:

        /* You can either pass the filename and size of the font to the
         * constructor, or pass a pre-loaded font bitmap object. */
        FontRenderer(const std::string& filename, unsigned int fontHeight);
        FontRenderer(const general::FontBitmap& fontBitmap);

        /* Cleans up the textures and display lists. */
        ~FontRenderer();

        /* Stream operator overloads for every type tha can be converted to
         * a string and drawn on the screen. */
        FontRenderer& operator<<(const short& val);
        FontRenderer& operator<<(const unsigned short& val);
        FontRenderer& operator<<(const int& val);
        FontRenderer& operator<<(const unsigned int& val);
        FontRenderer& operator<<(const long& val);
        FontRenderer& operator<<(const unsigned long& val);
        FontRenderer& operator<<(const float& val);
        FontRenderer& operator<<(const double& val);
        FontRenderer& operator<<(const long double& val);
        FontRenderer& operator<<(const void*& val);
        FontRenderer& operator<<(const char& val);
        FontRenderer& operator<<(const signed char& val);
        FontRenderer& operator<<(const unsigned char& val);
        FontRenderer& operator<<(const std::string& val);
        /* This overload changes the colour of any text drawn after it. */
        FontRenderer& operator<<(const colourf& newTextColour);

        /* Getters/setters for text layout. */
        float GetLineSpacing() const { return lineSpacing; }
        void SetLineSpacing(float lSpacing) { lineSpacing = lSpacing; }

        float GetHMargin() const { return hMargin; }
        void SetHMargin(float newMargin)
        {
            /* Sets new margin and updates the offset to so the text
             * drawn after this call will be drawn at the horizontal margin. */
            hMargin = newMargin;
            offset.x = hMargin;
        }
        float GetVMargin() const { return vMargin; }
        void SetVMargin(float newMargin)
        {
            // Same as SetHMargin(), but vertical margin instead
            vMargin = newMargin;
            offset.y = vMargin;
        }

        const maths::vector2f& GetOffset() const { return offset; }
        void SetOffset(const maths::vector2f& newOffset) { offset = newOffset; }




    };

}

}

#endif
