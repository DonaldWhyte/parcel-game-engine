/*
 * File:   TGAImage.h
 * Author: Donald
 *
 * Created on December 26, 2008, 11:18 AM
 */

#ifndef TGAIMAGE_H
#define TGAIMAGE_H

#include "Image.h"

namespace parcel
{

namespace graphics
{

    /* Class that loads pixel data from a TGA image file.
     * It only supports 24/32 bit true colour and grayscale modes. It does not
     * support colour index or 16 bit images. */
    class TGAImage : public Image
    {


        /* Nested enums/structs. */
    private:

        /* Stores all the values contained in the header of the TGA image.
         * This is used to determine the algorithm to use when loading the
         * pixel data. */
        struct TGAHeader
        {
            /* How much memory the TGA identifier string takes. Identifier is not really useful,
             * so this is only used to know how many bytes we have to skip over before reading pixel data. */
            unsigned char idLength;
            // 0 for true colour images and 1 for colur mapped images.
            unsigned char colourMapType;
            // Determines which type of TGA image it is. Types are in the TGAType enumerator.
            unsigned char imageTypeCode;
            // Indexed/colour map specific data
            unsigned char colourMapSpec[5];

            unsigned short xOrigin; // Origin of the X value
            unsigned short yOrigin; // Origin of the Y value
            unsigned short width; // Width (in pixels) of image
            unsigned short height; // Height (in pixels) of image
            // The amount of bits each pixel uses. Holds values of 8, 16, 24, or 32.
            unsigned char bitsPerPixel;
            /* Holds many bit fields that need to be extracted. Such as pixel orientation,
             * do the pxiels start at the top-left, bottom-right, etc. */
            unsigned char imageDescriptor;
        };

        /* Enum that contains the ID numbers for each type of TGA image.
         * Used to check which type is the loaded image is by checking a byte
         * inside the TGA Header. */
        enum TGAType
        {
            TGA_NoData = 0, // Blank targa file
            TGA_ColourMapped, // A colour-mapped/indexed colour file
            TGA_TrueColour, // True colour image
            TGA_Grayscale, // Blank and white image

            // Compressed versions of the above
            TGA_CompressedColourMapped = 9,
            TGA_CompressedTrueColour,
            TGA_CompressedGrayscale,
        };

        /* The data format of the image. */
        enum TGAImageDataFormat
        {
            TGAFORMAT_RGB = 0, // Has red, blue and green components
            TGAFORMAT_RGBA, // Has rgb valuesand an additonal alpha component
            TGAFORMAT_Luminance, // Is a grayscale image, singular values; 0 to 255
        };

        /* These constant variables are values of the flags that determine
         * where the image starts its pixel data. Used for checking a value
         * in the TGA Header to dtermien the origin of the data.*/
        static const unsigned char bottomLeft = 0x00;
        static const unsigned char bottomRight = 0x10;
        static const unsigned char topLeft = 0x20;
        static const unsigned char topRight = 0x30;


        /* Fields. */
        unsigned char colourDepth; // Depth of each colour (how many bits each a pixel uses)
        unsigned char imageDataFormat; // Format of the TGA image's data


        /* Rearranges the pixel data so the image is flipped vertically. This is done
         * because TGA images are sometimes saved upside down. */
        void FlipVertical();
        /* Swaps the red and blue components in the pixel data. */
        void SwapRedBlue();


        /* Methods. */
    public:

        /* Loads the pixel data from the file given. */
        bool LoadFromFile(const std::string& filename);
        /* Overload that includes the option to resize the image to the
         * nearest power-of-two size. */
        bool LoadFromFile(const std::string& filename, bool resizeToPow2);

        /* Useful methods that convert the image's format. */
        bool ConvertRGBToRGBA(unsigned char alpha);
        bool ConvertRGBAToRGB();


    };

}

}

#endif
