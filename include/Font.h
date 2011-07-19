 /*
 * File:   Font.h
 * Author: Donald "Datriot" Whyte
 *
 * Created on January 11, 2009, 2:06 PM
 */

#ifndef FONT_H
#define FONT_H

#include <string>
#include <GLee.h>
#include "Vector.h"
#include "RenderDevice.h"
#include "SkinManager.h"

namespace parcel
{

namespace graphics
{

    /* This class encapsulates the font bitmap loading and rendering code with
     * a simple interface. It also supports additional colours and custom sizes
     * when writing text.
     *
     * IMPORTANT: When loading a bitmap font, it not only requires the image file,
     * but also a .dff (Datriot Font File) text file to accompany it. It will have the real
     * widths of every character as well as the number of rows and columns of
     * characters in the actual image. This is used to generate the correct glyph data
     * while remaining flexible. It uses the following format:
     *
     * imagename <insert filename of image here>
     * width <insert width in pixels of image here>
     * height <insert height in pixels of image here>
     * rows <insert number of rows of characters>
     * columns <insert number of columns of characters>
     * A <insert real width in pixels of character A>
     * B <insert real width in pixels of character B>
     * etc.
     *
     * And you continue until you've specified all the characters and their real widths.
     * Order of the real widths have to be in the same order as the characters in
     * image, starting from the top left and ending at the bottom right.
     *
     * NOTE: This class is deprecated. There is a newer, more efficent font class
     * that provides an easier interface and more features.*/
    class Font
    {


    private:

        /* Holds information on a single glyph (also known as a character.) */
        struct Glyph
        {
            // The character that this glyph represents
            char character;
            // The width of the glyph, height is not needed since they are all on the same line
            unsigned int realWidth;
            /* Since each glyph has their own display list, this stores the glyph's
             * ID which is used to draw the display list. */
            GLuint displaylistID;

            Glyph() : character(' '), realWidth(0), displaylistID(0) {}
        };


        /* Fields. */

        // The maximum amount of possible glyphs
        static const unsigned int maxGlyphs = 200;

        /* Newline identifier - used to decide when to create a new line
         * We make this one higher than the maximum possible character
         * to make sure that it doesn't clash with any other character indexes. */
        unsigned int newlineID;


        Glyph* glyphs; // Array of all the font's glyphs
        unsigned int numGlyphs; // Amount of glyphs in font (also size of the above array)
        unsigned int glyphWidth; // Width of a character in the font (not the same as glyph's real width)
        unsigned int glyphHeight; // Height of a character in the font

        std::string fontTexture; // ID of the font's texture

        // Formatting variables
        bool equalSpacing; // Used when every letter is spaced by the same amount, regardless of its real width
        float characterSpacing; // How much space goes between each character
        float lineSpacing; // How large the space between lines is


        /* Private Methods. */

        /* This parses a string and converts it into an array of indexes to the glpyh array.
         * Returns false if it found a character it could not find in the glyph array. */
        bool GetGlyphIndexes(const std::string& text, unsigned int* glyphIndexes,
            unsigned int numIndexes);

        /* This draws the specified text. */
        void DrawText(RenderDevice* renderDevice, unsigned int* glyphIndexes,
            unsigned int numIndexes, const colourf& colour);


    public:

        /* Constructor, loads the texture and loads glyph data. After glyph data has
         * been created, use it to construct each glyph's display list. */
        Font(SkinManager* skinManager, std::string filename);
        ~Font();

        /* Deletes any display lists loaded and then releases glyph data. */
        void Release();


        /* All of these methods draw some text at the specified position using DrawText().
         * The overloads include changing the colour and size of the text.  */
        void Write(RenderDevice* renderDevice, const std::string& text,
            const maths::vector2f& position);
        void Write(RenderDevice* renderDevice, const std::string& text,
            const maths::vector2f& position, const maths::vector2f& size);
        void Write(RenderDevice* renderDevice, const std::string& text,
            const maths::vector2f& position, const colourf& size);
        void Write(RenderDevice* renderDevice, const std::string& text,
            const maths::vector2f& position, const maths::vector2f& size, const colourf& colour);


        /* These methods set the spacing between each character and line respectively. */
        void SetEqualSpacing (const bool& eqSpacing) { equalSpacing = eqSpacing; }
        void SetCharacterSpacing(const float& spacing) { characterSpacing = spacing; }
        void SetLineSpacing(const float& spacing) { lineSpacing = spacing; }


    };

}

}

#endif
