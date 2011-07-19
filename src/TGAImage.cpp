/*
 * File:   TGAImage.cpp
 * Author: Donald
 *
 * Created on December 26, 2008, 11:34 AM
 */

#include "TGAImage.h"
#include "Colour.h"
#include "Exceptions.h"
#include "MCommon.h"

namespace parcel
{

namespace graphics
{

    bool TGAImage::LoadFromFile(const std::string& filename)
    {
        // Opens the file
        FILE* file = fopen(filename.c_str(), "rb");
        // If file failed to load, return false
        if (!file) return false;

        TGAHeader header; // The header of the TGA
        // Read the TGA header from the file and store in the header struct
        fread(&header, 1, sizeof(TGAHeader), file);

        /* Closes file and returns false if the image is a colour map or has no data.
         * NOTE: This doesn't and probably never will support indexed colour maps. */
         if (header.imageTypeCode == TGA_NoData ||
            header.imageTypeCode == TGA_ColourMapped ||
            header.imageTypeCode == TGA_CompressedColourMapped)
        {
            fclose(file);
            return false;
        }

        // Stores width and height of image
        width = header.width;
        height = header.height;

        // colourMode is 3 = BGR, colourMode is 4 = BGRA
        int colourMode = header.bitsPerPixel / 8;
        /* If colour mode is less than 3, then that means that there is less than
         * 24 bits per pixel, and this loader does not support that. */
        if (colourMode < 3)
        {
            fclose(file);
            return false;
        }

        imageSize = (width * height * colourMode); // Calculates size of image in memory
        pixelData = new unsigned char[imageSize]; // Allocates calculated memory to store pixel data

        // Skip past the ID string if there is one
        if (header.idLength > 0) fseek(file, SEEK_CUR, header.idLength);

        // Reads image data and stores it in pixelData
        if (header.imageTypeCode == TGA_TrueColour ||
            header.imageTypeCode == TGA_Grayscale)
        {
            // If file is NOT compressed, simply read the entire file and store it in the pixelData array
            fread(pixelData, 1, imageSize, file);
        }
        else // If compressed )=
        {
            unsigned int i = 0; // Iterator
            unsigned char id, length;
            Colour<unsigned char> colour(0, 0, 0, 0); // Will contain the colour

            while (i < imageSize)
            {
                id = fgetc(file); // Gets character in the file

                // Checks if this is run length data
                if (id >= 128)
                {
                    // Find the run length
                    length = (unsigned char)(id - 127);

                    // Assigns colour values
                    colour.b = (unsigned char)fgetc(file);
                    colour.g = (unsigned char)fgetc(file);
                    colour.r = (unsigned char)fgetc(file);
                    if (colourMode == 4) colour.a = (unsigned char)fgetc(file); // If alpha is supported, add alpha too

                    // Loop until the length is greater than 0
                    while(length > 0)
                    {
                        // Puts this pixel's colour in the pixel data array
                        pixelData[i++] = colour.b;
                        pixelData[i++] = colour.g;
                        pixelData[i++] = colour.r;

                        if (colourMode == 4) pixelData[i++] = colour.a;


                        --length; // Lowers length by one
                    }
                }
                else
                {
                    // The number of non RLE (compressed) pixels
                    length = (unsigned char)(id + 1);

                    while(length > 0)
                    {
                        // Assigns colour values
                        colour.b = (unsigned char)fgetc(file);
                        colour.g = (unsigned char)fgetc(file);
                        colour.r = (unsigned char)fgetc(file);
                        if (colourMode == 4) colour.a = (unsigned char)fgetc(file);
                        // Puts this pixel's colour in the pixel data array
                        pixelData[i++] = colour.b;
                        pixelData[i++] = colour.g;
                        pixelData[i++] = colour.r;
                        if (colourMode == 4) pixelData[i++] = colour.a;

                        --length;
                    }
                }
            }
        }

        // We're done reading from the image, so we close the file
        fclose(file);

        // Checks which type of image it is to set the image's colour depth and data formats
        switch (header.imageTypeCode)
        {
            // If it is a true colour image
            case TGA_TrueColour:
            case TGA_CompressedTrueColour:
                if (colourMode == 3) // Uses red, blue and green
                {
                    imageDataFormat = TGAFORMAT_RGB;
                    colourDepth = 24;
                }
                else // Otherwise, it must use alpha too
                {
                    imageDataFormat = TGAFORMAT_RGBA;
                    colourDepth = 32;
                }
                break;

            // If it is a grayscale (or luminance) image
            case TGA_Grayscale:
            case TGA_CompressedGrayscale:
                imageDataFormat = TGAFORMAT_Luminance;
                colourDepth = 8;
                break;

        }

        /* If pixels start at the top left, flip it vertically, since it will be used
         * for OpenGL, and that starts from the bottom left. OPENGL */
        if ((header.imageDescriptor & topLeft) == topLeft) FlipVertical();

        // Swap the red and blue components in the pixel data so it is ordered correctly for OPENGL
        SwapRedBlue();

        // Returns the success of the image load
        return (pixelData != NULL);
    }

    bool TGAImage::LoadFromFile(const std::string& filename, bool resizeToPow2)
    {
        // Used to iterate through 'pixelData' and set appropriate values
        unsigned int i = 0;

        FILE* file = fopen(filename.c_str(), "rb");
        if (!file) return false;

        TGAHeader header;
        fread(&header, 1, sizeof(TGAHeader), file);

         if (header.imageTypeCode == TGA_NoData ||
            header.imageTypeCode == TGA_ColourMapped ||
            header.imageTypeCode == TGA_CompressedColourMapped)
        {
            fclose(file);
            return false;
        }

        // Stores width and height of image
        width = header.width;
        height = header.height;
        /* These two variables contain the REAL width of the image, when it's been resized
         * to power-of-two or not. */
        unsigned int realWidth = width, realHeight = height;
        // If 'resizeToPow2' flag is set, sets width and height to the nearest power-of-two
        if (resizeToPow2)
        {
            realWidth = maths::ClosestPow2(width);
            realHeight = maths::ClosestPow2(height);
        }

        int colourMode = header.bitsPerPixel / 8;
        if (colourMode < 3)
        {
            fclose(file);
            return false;
        }

        imageSize = (width * height * colourMode);
        unsigned int realImageSize = (realWidth * realHeight * colourMode);
        pixelData = new unsigned char[realImageSize];

        if (header.idLength > 0) fseek(file, SEEK_CUR, header.idLength);

        if (header.imageTypeCode == TGA_TrueColour ||
            header.imageTypeCode == TGA_Grayscale)
        {
            fread(pixelData, 1, imageSize, file);
        }
        else
        {
            i = 0; // Resets iterator
            unsigned char id, length;
            Colour<unsigned char> colour(0, 0, 0, 0);

            while (i < imageSize)
            {
                id = fgetc(file);

                if (id >= 128)
                {
                    length = (unsigned char)(id - 127);

                    colour.b = (unsigned char)fgetc(file);
                    colour.g = (unsigned char)fgetc(file);
                    colour.r = (unsigned char)fgetc(file);
                    if (colourMode == 4) colour.a = (unsigned char)fgetc(file);

                    while(length > 0)
                    {
                        pixelData[i++] = colour.b;
                        pixelData[i++] = colour.g;
                        pixelData[i++] = colour.r;

                        if (colourMode == 4) pixelData[i++] = colour.a;

                        --length;
                    }
                }
                else
                {
                    length = (unsigned char)(id + 1);

                    while(length > 0)
                    {
                        colour.b = (unsigned char)fgetc(file);
                        colour.g = (unsigned char)fgetc(file);
                        colour.r = (unsigned char)fgetc(file);
                        if (colourMode == 4) colour.a = (unsigned char)fgetc(file);
                        pixelData[i++] = colour.b;
                        pixelData[i++] = colour.g;
                        pixelData[i++] = colour.r;
                        if (colourMode == 4) pixelData[i++] = colour.a;

                        --length;
                    }
                }
            }
        }

        fclose(file);

        // If the image has been resized to nearest power-of-two...
        if (resizeToPow2)
        {
            i = imageSize; // Sets iterator to end of real image data
            // Fills the rest of the image with zeroes for black/transparent pixels
            while (i < realImageSize)
            {
                pixelData[i++] = 0;
            }
        }

        switch (header.imageTypeCode)
        {
            case TGA_TrueColour:
            case TGA_CompressedTrueColour:
                if (colourMode == 3)
                {
                    imageDataFormat = TGAFORMAT_RGB;
                    colourDepth = 24;
                }
                else
                {
                    imageDataFormat = TGAFORMAT_RGBA;
                    colourDepth = 32;
                }
                break;

            case TGA_Grayscale:
            case TGA_CompressedGrayscale:
                imageDataFormat = TGAFORMAT_Luminance;
                colourDepth = 8;
                break;

        }

        if ((header.imageDescriptor & topLeft) == topLeft) FlipVertical();
        SwapRedBlue();
        return (pixelData);
    }

    bool TGAImage::ConvertRGBAToRGB()
    {
        // Checks that the image is indeed a 32-bit RGBA image
        if ((colourDepth == 32) && (imageDataFormat == TGAFORMAT_RGBA))
        {
            // The new pixel data will be stored here
            unsigned char* newPixelData = new unsigned char[width * height * 3];
            unsigned int i = 0; // Used to iterate through the new pixel data
            unsigned int j= 0; // Used to iterate through old pixel data

            // Loops through every pixel and gets thed red, green and blue components
            for (unsigned int x = 0; (x < height); x++)
            {
                for (unsigned int y = 0; (y < width); y++)
                {
                    newPixelData[i++] = pixelData[j++];
                    newPixelData[i++] = pixelData[j++];
                    newPixelData[i++] = pixelData[j++];
                    j++; // Skips over the old pixel data's alpha component
                }
            }

            // Deletes pixel data and assigns the pointer to the new pixel data
            delete[] pixelData;
            pixelData = (unsigned char*)newPixelData;
            // Changes the image's format to RGB
            colourDepth = 24;
            imageDataFormat = TGAFORMAT_RGB;

            return true; // Conversion was a success!
        }

        // Otherwise, it cannot be converted, so we return false
        else return false;
    }

    bool TGAImage::ConvertRGBToRGBA(unsigned char alpha)
    {
        // Checks that the image is a 24bit RGB image
        if ((colourDepth == 24) && (imageDataFormat == TGAFORMAT_RGB))
        {
            unsigned char* newPixelData = new unsigned char[width * height * 4];
            unsigned int i = 0;
            unsigned int j = 0;

            for (unsigned int x = 0; (x < height); x++)
            {
                for (unsigned int y = 0; (y < height); y++)
                {
                    newPixelData[i++] = pixelData[j++];
                    newPixelData[i++] = pixelData[j++];
                    newPixelData[i++] = pixelData[j++];
                    newPixelData[i++] = alpha; // Sets the alpha component of every colour to the one given
                }
            }

            // Deletes pixel data and assigns the pointer to the new pixel data
            delete[] pixelData;
            pixelData = (unsigned char*)newPixelData;
            // Changes the image's format to RGBA
            colourDepth = 32;
            imageDataFormat = TGAFORMAT_RGBA;

            return true; // Conversion was a success!
        }

        else return false;
    }

    void TGAImage::FlipVertical()
    {
        // Checks if pixel data exists
        if (!pixelData) throw debug::NullPointerException("TGAImage::FlipVertical - Pixel data is a null pointer!");

        // Checks whther image is RGB or RGBA
        int colourMode;

        if (colourDepth == 32) colourMode = 4; // RGBA
        else if (colourDepth == 24) colourMode = 3; //RGB
        else return; // Any other depths are ignored and the method returns

        // Temporary storage of the pixel data
        unsigned char* tempPixelData = new unsigned char[width * colourMode];
        if (!tempPixelData) // throws exception if allocation failed
            throw debug::OutOfMemoryException("TGAImage::FlipVertical - Failed to allocate memory for temporary pixel data!");

        // Calculates how many bytes there is for a single line
        int lineWidth = (width * colourMode);

        // Theses hold the top and bottom of the image's pixel data
        unsigned char* top = (unsigned char*)pixelData;
        unsigned char* bottom = (unsigned char*)(pixelData + lineWidth * (height - 1));

        // Copies all the data appropriately to flip the image abd nake it upside down
        for (unsigned int i = 0; (i < height / 2); i++)
        {
            memcpy(tempPixelData, top, lineWidth); // Copies top data to temp pointer
            memcpy(top, bottom, lineWidth); // Copies bottom data to top pointer
            memcpy(bottom, tempPixelData, lineWidth); // Copies temp data to bottom pointer

            top = (unsigned char*)(top + lineWidth);
            bottom = (unsigned char*)(bottom - lineWidth);
        }

        // Deletes the temporary pixel data
        delete[] tempPixelData;
        tempPixelData = NULL;
    }

    void TGAImage::SwapRedBlue()
    {
        // Checks if pixel data exists
        if (!pixelData) throw debug::NullPointerException("TGAImage::FlipVertical - Pixel data is a null pointer!");

        unsigned char temp; // Temporary container for a colour component

        switch (colourDepth)
        {
            // If the image uses 32 bit colours, meaning they use red, green, blue and alpha components
            case 32:
                // Iterates through the entire array, swapping the red and blue components of each pixel
                for (unsigned int i = 0; i < (width * height * 4); i += 4)
                {
                    temp = pixelData[i];
                    pixelData[i] = pixelData[i + 2];
                    pixelData[i + 2] = temp;
                }
                break;

            // If the image uses 24 bit colours, meaning they only have red, green and blue components
            case 24:
                // Iterates through the entire array, swapping the red and blue components of each pixel
                for (unsigned int i = 0; i < (width * height * 3); i += 3)
                {
                    temp = pixelData[i];
                    pixelData[i] = pixelData[i + 2];
                    pixelData[i + 2] = temp;
                }

                break;
            // Simply ignore all the other colour depths
            default: break;
        }
    }

}

}
