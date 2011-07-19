 /*
 * File:   Font.cpp
 * Author: Donald "Datriot" Whyte
 *
 * Created on January 11, 2009, 2:56 PM
 */

#include <GLee.h>
#include <fstream>
#include "Font.h"
#include "Vector.h"
#include "Exceptions.h"

namespace parcel
{

namespace graphics
{

    Font::Font(SkinManager* skinManager, std::string filename)
        : equalSpacing(false), characterSpacing(0.1f)
    {
        try
        {
            // First of all, let's load the dff file
            std::ifstream file;
            file.open(filename.c_str());
            // Makes sure that the file was loaded successfully and skin manager isn't null
            if (!file)
            {
                throw debug::NullPointerException("Font::Font - Could not load the dff file!");
            }
            if (!skinManager)
            {
                throw debug::NullPointerException("Font::Font - Skin Manager is a null pointer!");
            }

            // This will hold a single line in the file
            char buffer[100];
            // Used to iterate through file for real widths
            unsigned int i;

            // Name of the image to load
            char imageName[100];
            // The dimensions of the image
            unsigned int width, height;

            // How many rows and columns of characters there are in the image
            unsigned int rows, columns;
            /* The real widths of each character. Stores data of characters in the same
             * order as the characters in the image. This order is specified in Font.h. */
            unsigned int realWidths[maxGlyphs]; // Can hold a maximum of 200 real widths
            /* Determines which characters go with which real widths. Also acts
             * as a temporary container for the all the characters in the font. */
            char characterOrder[maxGlyphs]; // Can hold a maximum of 200 characters

            // Gets the first line and tries to image name from this line
            file.getline(buffer, 100);
            sscanf(buffer, "%*s %s", imageName);
            // Gets the width and height of the image
            file.getline(buffer, 100);
            sscanf(buffer, "%*s %d", &width);
            file.getline(buffer, 100);
            sscanf(buffer, "%*s %d", &height);
            // Gets the amount of rows and columns from the file
            file.getline(buffer, 100);
            sscanf(buffer, "%*s %d", &rows);
            file.getline(buffer, 100);
            sscanf(buffer, "%*s %d", &columns);

            /* This determines how many glyphs are generated and is used to
             * allocate memory for those glyphs. */
            numGlyphs = 0;
            i = 0;

            /* Now the rest of the lines will be the individual character (glyph) widths.
             * To get them, we loop until the end of the file has been reached. */
            while (!file.eof())
            {
                // Breaks loop if amount of real widths is
                if (i > maxGlyphs) break;

                file.getline(buffer, 100);
                /* Attempts to find width of the character, and then increases the iterator
                 * Also stores the actual character too. */
                sscanf(buffer, "%c %d", &characterOrder[i], &realWidths[i]);

                // Increases the amount of glyphs and iterator
                ++numGlyphs;
                ++i;
            }

            // We're done with the file, so let's close it
            file.close();

            // Determines ID of the texture
            fontTexture = imageName;
            /* Tries to load the texture using gotten image name. Upon failure, throw an exception. */
            if (!skinManager->AddTexture(fontTexture, imageName, 1.0f))
                throw debug::Exception("Font::Font - Texture creation for font failed.");




            // Now we have aquired the data from the dff file, we must generate the glyph data

            // Allocates the needed memory for the glyph data
            glyphs = new Glyph[numGlyphs];

            /* Searches through all the character and real width data aquired and puts them
             * in an array of glyphs. */
            for (unsigned int j = 0; (j < numGlyphs); j++)
            {
                glyphs[j].character = characterOrder[j];
                glyphs[j].realWidth = realWidths[j];
            }




            /* Now that glyph data has been generated, we create a display list for every glyph
             * that will be used to render those glyphs quickly and easily. This is where the use
             * of glyphWidth/Height comes in. */

            // Calculates the dimensions of every glyph
            glyphWidth = (width / columns);
            glyphHeight = (height / rows);

            // Generates the display lists and stores the first ID in this integer
            GLuint displaylists = glGenLists(numGlyphs);

            if (displaylists == 0)
            {
                throw debug::Exception("Font::Font - No display lists were generated! Cannot create font!");
            }

            // Used to iterate through display list ID and glyph data
            unsigned int k = 0;

            // Creates a new list for every glyph, calculating the texcoords and vertices as it goes along
            for (unsigned int r = 0; (r < rows); r++)
            {
                for (unsigned int c = 0; (c < columns); c++)
                {
                    // Break the loop if it's generating more display lists than needed
                    if (k >= numGlyphs) break;

                    maths::vector2f v1, v2, v3, v4, t1, t2, t3, t4;
                    // Calculates the correct vertices
                    v1.x = v1.y = v2.y = v4.x = 0.0f;
                    v2.x = v3.x = glyphs[k].realWidth;
                    v3.y = v4.y = glyphHeight;
                    /* Calculates the appropriate texture cooridnates.
                     * The caluclations are reversed, so the top is bottom and the bottom
                     * is the top. This is because texture cooridnates start at the bottom left,
                     * but the font image's layout starts at the top left. */
                    t1.x = (float)(glyphWidth * c) / width;
                    t1.y = (float)(height - ((glyphHeight * r) + glyphHeight)) / height; // REVERSED!!
                    t2.x = (float)((glyphWidth * c) + glyphs[k].realWidth) / width;
                    t2.y = (float)(height - ((glyphHeight * r) + glyphHeight)) / height; // REVERSED!!
                    t3.x = (float)((glyphWidth * c) + glyphs[k].realWidth) / width;
                    t3.y = (float)(height - (glyphHeight * r)) / height; // REVERSED!!
                    t4.x = (float)(glyphWidth * c) / width;
                    t4.y = (float)(height - (glyphHeight * r)) / height; // REVERSED!!
                    // ^^^ Since every glyph has same height, just use glyphHeight ^^^

                    // Now generates display list using the calculated texture coordinates
                    glNewList((displaylists + k), GL_COMPILE);
                      glBegin(GL_QUADS);
                        glTexCoord2fv(t1.values);
                        glVertex2fv(v1.values);

                        glTexCoord2fv(t2.values);
                        glVertex2fv(v2.values);

                        glTexCoord2fv(t3.values);
                        glVertex2fv(v3.values);

                        glTexCoord2fv(t4.values);
                        glVertex2fv(v4.values);
                      glEnd();
                    glEndList();

                    // Assigns ID of newly created list to the appropriate glyph
                    glyphs[k].displaylistID = (displaylists + k);

                    ++k;
                }

                // Break this loop as well
                if (k >= numGlyphs) break;
            }

            // Newline identifier is the amount of glyphs plus one, to make sure ther are no conflicts
            newlineID = numGlyphs + 1;
            // Finally, sets line spacing to the height of a glyph and add a little more
            lineSpacing = 0.1f;
        }
        catch (debug::Exception ex)
        {
            // Prints error message and performs neccesery cleanup
            std::cout << ex.Message();
            Release();
        }
        catch (std::exception ex)
        {
            std::cout << "Font::Font - std::exception occured. Problem is unknown, aborting font creation.";
            Release();
        }
        catch(...) { Release(); } // Catches anything, cleans up if neccesery
    }

    Font::~Font()
    {
        // Releases any resources used by this class
        Release();
    }


    void Font::Release()
    {
        if (glyphs)
        {
            // Deletes all the display lists used by this font
            for (unsigned int i = 0; (i < numGlyphs); i++)
            {
                // Makes sure display exists exists before attempting to delete it
                if (glIsList(glyphs[i].displaylistID))
                {
                    glDeleteLists(glyphs[i].displaylistID, 1);
                }
            }

            // Releases resources used to store glyph data
            delete[] glyphs;
            glyphs = NULL;
        }
    }



    bool Font::GetGlyphIndexes(const std::string& text, unsigned int* glyphIndexes,
        unsigned int numIndexes)
    {
        // Converts the string into a char array
        const char* charArray = text.c_str();
        // Searches through string to find correct glyph data
        for (unsigned int i = 0; (i < text.size()); i++)
        {
            // Breaks the loop if it has exceeded the amount of space in the array
            if (i >= numIndexes) break;
            /* Gives index a default value of maxGlyphs + 1. If it cannot find a matching
             * character, then it stays at this value. So afterafter the following loop we
             * check if it is still maxGlyphs + 1, it couldn't find the character. So we
             * print and error message and return false. */
            glyphIndexes[i] = maxGlyphs + 1;

            for (unsigned int j = 0; (j < numGlyphs); j++)
            {
                // If newline escape sequence detected, make the index the newline identifier
                if (charArray[i] == '\n')
                    glyphIndexes[i] = newlineID;
                else if (text.c_str()[i] == glyphs[j].character) // If character matches the glyph's
                    glyphIndexes[i] = j; // Then set index to where that glyph resides
            }

            // Now checks if it found a glyph
            if (glyphIndexes[i] == maxGlyphs + 1)
            {
                // It never found the a matching glyph! Let's abort the writing!
                std::cout << "Font::GetGlyphIndexes - Error! Couldn't find a glyph to match character!" << std::endl
                    << "\tFont writing has been aborted!" << std::endl;
                return false;
            }
        }

        return true; // Success!
    }



    void Font::DrawText(RenderDevice* renderDevice, unsigned int* glyphIndexes,
        unsigned int numIndexes, const colourf& colour)
    {
        // Stores the amount of line breaks that have occured
        unsigned int numLines = 0;


        // Sets the font texture to the currently active one
        renderDevice->SetActiveTextureWithColourMask(fontTexture, colour);


        glMatrixMode(GL_MODELVIEW);
        glPushMatrix(); // Stores current matrix


        // Now, renders each character
        for (unsigned int i = 0; (i < numIndexes); i++)
        {
            /* If the next character is a new line, pop the matrix to restore
             * horizontal position, then push the matrix again and translate
             * downwards for a new line. */
            if (glyphIndexes[i] == newlineID)
            {
                ++numLines; // Increases the amount of broken lines

                glPopMatrix(); // Restores character position
                glPushMatrix(); // And saves the current position again
                // Moves character position downwards depending on the amount of broken lines
                glTranslatef(0.0f, -((glyphHeight + lineSpacing) * numLines), 0.0f);
            }
            else // Otherwise, just draw the character
            {
                Glyph glyph = glyphs[glyphIndexes[i]];

                // Draws character
                glCallList(glyph.displaylistID);
                // Translates for the next character's position
                // For equal spacing, use the same width for every glyph, which is just the spacing value
                if (equalSpacing)
                    glTranslatef(characterSpacing, 0.0f, 0.0f);
                else // Otherwise, use the glyph's real width
                    glTranslatef((glyph.realWidth + characterSpacing), 0.0f, 0.0f);
            }
        }
        /* NOTE: Get rid of visual artifacts when text is resized, maybe use mipmaps.
         * Probably will never get done since a newer font system has been created. */

        glPopMatrix(); // Restores the matrix before rendering begun
    }



    void Font::Write(RenderDevice* renderDevice, const std::string& text,
        const maths::vector2f& position)
    {
        unsigned int numIndexes = text.size(); // Size of index array
        unsigned int glyphIndexes[numIndexes]; // Stores all the indexes to the glyphs

        /* Parses string and puts glpyh indexes in the glyphIndexes array.
         * If it failed to get the glyph indexes, return to avoid further errors. */
        if (!GetGlyphIndexes(text, glyphIndexes, numIndexes)) return;


        // Makes sure we're using the right matrix stack
        glMatrixMode(GL_MODELVIEW);

        // Before rendering with DrawText(), translate text to specified position.
        glPushMatrix(); // Stores current matrix
        glTranslatef(position.x, position.y, 0.0f); // Translates the text to the specified position

        /* Now that the text is positioned correctly, draws all the glyphs needed.
         * uses white as default colour mask, which won't alter the texture. */
        DrawText(renderDevice, glyphIndexes, numIndexes, presetcolours::White);

        // Restores original matrix, before we started rendering
        glPopMatrix();
    }
    void Font::Write(RenderDevice* renderDevice, const std::string& text,
        const maths::vector2f& position, const maths::vector2f& size)
    {
        unsigned int numIndexes = text.size();
        unsigned int glyphIndexes[numIndexes];

        if (!GetGlyphIndexes(text, glyphIndexes, numIndexes)) return;


        glMatrixMode(GL_MODELVIEW);

        /* Before rendering with DrawText(), translate text to specified position and resize
         * appropriately. */
        glPushMatrix();

        glTranslatef(position.x, position.y, 0.0f); // Translates the text to the specified position
        glScalef(size.x, size.y, 0.0f); // Scale

        DrawText(renderDevice, glyphIndexes, numIndexes, presetcolours::White);

        glPopMatrix();
    }
    void Font::Write(RenderDevice* renderDevice, const std::string& text,
        const maths::vector2f& position, const colourf& colour)
    {
        unsigned int numIndexes = text.size();
        unsigned int glyphIndexes[numIndexes];

        if (!GetGlyphIndexes(text, glyphIndexes, numIndexes)) return;


        glMatrixMode(GL_MODELVIEW);

        glPushMatrix();

        glTranslatef(position.x, position.y, 0.0f);

        // Draws the text with the custom colour given
        DrawText(renderDevice, glyphIndexes, numIndexes, colour);

        glPopMatrix();
    }
    void Font::Write(RenderDevice* renderDevice, const std::string& text,
        const maths::vector2f& position, const maths::vector2f& size, const colourf& colour)
    {
        unsigned int numIndexes = text.size();
        unsigned int glyphIndexes[numIndexes];

        if (!GetGlyphIndexes(text, glyphIndexes, numIndexes)) return;


        glMatrixMode(GL_MODELVIEW);

        glPushMatrix();

        glTranslatef(position.x, position.y, 0.0f);
        glScalef(size.x, size.y, 0.0f);

        // Draws the text with the custom colour given
        DrawText(renderDevice, glyphIndexes, numIndexes, colour);

        glPopMatrix();
    }

}

}
