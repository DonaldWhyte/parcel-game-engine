/*
 * File:   FontRenderer.cpp
 * Author: Donald "Datriot" Whyte
 *
 * Created on May 5, 2009, 9:21 AM
 */

#include <string>
#include <GLee.h>
#include "FontRenderer.h"
#include "MCommon.h"
#include "Util.h"

namespace parcel
{

namespace graphics
{

    FontRenderer::FontRenderer(const std::string& filename, unsigned int fontHeight) :
        height(fontHeight), hMargin(0.0f), vMargin(0.0f), lineSpacing(0.0f),
        freshLine(true), offset(hMargin, vMargin), textColour(1.0f, 1.0f, 1.0f, 1.0f)
    {
        // Creates the font object and then passes it to the Initialise() method
        Initialise(general::FontBitmap(filename, fontHeight));
    }

    FontRenderer::FontRenderer(const general::FontBitmap& fontBitmap) :
        height(fontBitmap.GetHeight()), hMargin(0.0f), vMargin(0.0f), lineSpacing(0.0f),
        freshLine(true), offset(hMargin, vMargin), textColour(1.0f, 1.0f, 1.0f, 1.0f)
    {
        // Simply passes the given bitmap font to Initialise() to let it handle the rest
        Initialise(fontBitmap);
    }

    FontRenderer::~FontRenderer()
    {
        // Deletes the textures and display lists used to draw the font
        glDeleteTextures(numGlyphs, textures);
        glDeleteLists(displayLists, numGlyphs);
    }


    void FontRenderer::Initialise(const general::FontBitmap& fontBitmap)
    {
        // Contains data on every glyph in the font
        glyphs = fontBitmap.GetGlyphs();
        numGlyphs = glyphs.size();
        /* Creates a vertex array containing vertices and texture
         * coordinates for every glyph. */
        std::vector<float> geometryData = fontBitmap.CreateVertexArray();

        /* Activates vertex and texture arrays. This is used to build every glyph's
         * display list correctly. */
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        // Uses geometryData (acquired from FontBitmap object) as vertex data
        glVertexPointer(2, GL_FLOAT, (sizeof(float) * 4), &geometryData[0]);
        glTexCoordPointer(2, GL_FLOAT, (sizeof(float) * 4), &geometryData[2]);

        // Generates enough textures and display lists for every glyph
        glGenTextures(numGlyphs, &textures[0]);
        displayLists = glGenLists(numGlyphs);
        // Fills all of the textures and display lists in for every glyph
        for (unsigned char i = 0; (i < numGlyphs); i++)
        {
            LoadTexture(glyphs[i], i);
            BuildDisplayList(glyphs[i], i);
        }

        // Disables the vertex arrays, we don't need the use the geometry data anymore
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }


    void FontRenderer::LoadTexture(const general::Glyph& glyph, unsigned char character)
    {
        // Binds the created texture and sets it parameters
        glBindTexture(GL_TEXTURE_2D, textures[character]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        // Then loads the glyph's pixel data using luminance format
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glyph.powWidth, glyph.powHeight, 0,
            GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, &glyph.bitmap[0]);
    }

    void FontRenderer::BuildDisplayList(const general::Glyph& glyph, unsigned char character)
    {
        // Creates the display list and binds the appropriate texture to it
        glNewList(displayLists + character, GL_COMPILE);
        glBindTexture(GL_TEXTURE_2D, textures[character]);

        glPushMatrix();
            // Rotates glyph to get it appear the right side up
            glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
            // Correctly positions the glyph
            glTranslatef(glyph.left, 0, 0);
            glTranslatef(0, glyph.underBase, 0); // Positions it correctly on the baseline
            // Draws 24 vertices at the correct index, this will render a single glyph
            glDrawArrays(GL_QUADS, character * 4, 4);
        glPopMatrix();

        // Translates to the right so next character is in the right position
        glTranslatef(glyph.advanceX, 0, 0);
        // We are finished creating the display list
        glEndList();
    }

    std::vector<std::string> FontRenderer::ParseLines(const std::string& text)
    {
        std::vector<std::string> lines; // Stores created strings
        const char* startLine = text.c_str(); // Where the next line's creation will start
        const char* c; // Used to iterator through characters in the string

        // Iterates through every character in the string
        for (c = &text[0]; *c; c++)
        {
            if (*c == '\n')
            {
                // Creates a new string for the line
                std::string line;
                // Copies the line's contents from original string into the new one
                for (const char* n = startLine; (n < c); n++)
                {
                    line.append(1, *n);
                }
                // Adds it to the list
                lines.push_back(line);
                // Adjusts character offset to start at the beginning of the next line
                startLine = (c + 1);
            }
        }
        // If there is only one line left, make it its own string and add it the list
        if (startLine)
        {
            // Fills the line with the remainader of the string
            std::string line;
            for (const char* n = startLine; (n < c); n++)
            {
                line.append(1, *n);
            }
            lines.push_back(line);
        }

        return lines;
    }


    void FontRenderer::Draw(const std::string& text)
    {
        // Splits every line of text in the string into a seperate string
        std::vector<std::string> lines = ParseLines(text);

        // Now draws the text
        // Start by setting the base of the display lists
        glListBase(displayLists);
        // Sets colour of the text
        glColor4fv(textColour.values);
        /* Makes sure drawing offset is set at the current margin. This is to
         * align and draw the text with the desired margin. */
        if (freshLine)
        {
            offset.x = hMargin;
            // Fresh line's x position has been reset, to clear the flag
            freshLine = false;
        }

        // Draws each line in order
        for (unsigned int i = 0; (i < lines.size()); i++)
        {
            // Stores current matrix and then clears the matrix
            glPushMatrix();
            // Positions the character where it is supposed to be
            glTranslatef(offset.x, offset.y, 0);
            // Draws the display lists of e very character
            glCallLists(lines[i].length(), GL_UNSIGNED_BYTE, lines[i].c_str());
            // Restores the current matrix stack, since we are done drawing
            glPopMatrix();

            // More positioning stuff
            // Shifts the offset to the END of the text we were just writing
            for (unsigned int j = 0; (j < lines[i].length()); j++)
            {
                offset.x += glyphs[lines[i].c_str()[j]].advanceX;
            }
            // Unless it is the last line, move offset down to draw next line
            if (i != (lines.size() - 1))
            {
                // Also resets the X coordinate to the margin
                offset.x = hMargin;
                // Moves the offset down by one line
                offset.y -= (height / 0.63f);
                // Adds any extra spacing using lineSpacing
                offset.y -= lineSpacing;
            }
            /* If it is the last line and it is blank, we can set the freshLine
             * flag to true, which makes future text draw in the correct position. */
            else if (i == (lines.size() - 1) && lines[i].empty())
            {
                freshLine = true;
            }
        }

        // Resets display list base to 0
        glListBase(0);
    }


    /* Implementations of all the draw operator<< overloads. */
    FontRenderer& FontRenderer::operator<<(const short& val)
    {
        // Converts parameter to string and draws it
        Draw(general::ToString(val));
        return *this;
    }
    FontRenderer& FontRenderer::operator<<(const unsigned short& val)
    { Draw(general::ToString(val)); return *this; }
    FontRenderer& FontRenderer::operator<<(const int& val)
    { Draw(general::ToString(val)); return *this; }
    FontRenderer& FontRenderer::operator<<(const unsigned int& val)
    { Draw(general::ToString(val)); return *this; }
    FontRenderer& FontRenderer::operator<<(const long& val)
    { Draw(general::ToString(val)); return *this; }
    FontRenderer& FontRenderer::operator<<(const unsigned long& val)
    { Draw(general::ToString(val)); return *this; }
    FontRenderer& FontRenderer::operator<<(const float& val)
    { Draw(general::ToString(val)); return *this; }
    FontRenderer& FontRenderer::operator<<(const double& val)
    { Draw(general::ToString(val)); return *this; }
    FontRenderer& FontRenderer::operator<<(const long double& val)
    { Draw(general::ToString(val)); return *this; }
    FontRenderer& FontRenderer::operator<<(const void*& val)
    { Draw(general::ToString(val)); return *this; }
    FontRenderer& FontRenderer::operator<<(const char& val)
    { Draw(general::ToString(val)); return *this; }
    FontRenderer& FontRenderer::operator<<(const signed char& val)
    { Draw(general::ToString(val)); return *this; }
    FontRenderer& FontRenderer::operator<<(const unsigned char& val)
    { Draw(general::ToString(val)); return *this; }
    FontRenderer& FontRenderer::operator<<(const std::string& val)
    { Draw(val); return *this; }

    /* Change the colour of any text drawn AFTER this. */
    FontRenderer& FontRenderer::operator<<(const colourf& newTextColour)
    {
        textColour = newTextColour;
        return *this;
    }

}

}
