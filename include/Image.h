/*
 * File:   Image.h
 * Author: Donald
 *
 * Created on December 26, 2008, 11:06 AM
 */

#ifndef IMAGE_H
#define IMAGE_H

#include <string>

namespace parcel
{

namespace graphics
{

    /* Abstract image class. All other image classes derive from this, such as TGAImage or PNGImage. */
    class Image
    {

    protected:
        unsigned char* pixelData; // The actual pixel data of the image
        unsigned int width, height; // Width and height of image (in pixels)
        unsigned long imageSize; // Size of image, in memory, not in pixels

    public:

        /* Constructor, intiailzies the pixelData pointer and width/height values */
        Image() : pixelData(NULL), width(0), height(0), imageSize(0) {}
        /* Releases any resources used by the class, such as pixelData. It is made
         * virtual so any derived classes can release any extra resources they may
         * have used. */
        virtual ~Image()
        {
            delete[] pixelData;
            pixelData = NULL;
        }

        /* Pure virtual function. Derived classes overload this function to perform loading
         * algorithms specific to their image format. Loads pixel data from the filename given.
         * Overloaded functinos return a boolean that reports success of file loading, true is
         * file loaded sucessfully, false it never. */
        virtual bool LoadFromFile(const std::string& filename) = 0;

        /* These two functions convert an RGB image to an RGBA image or vice versa.
         * Useful is the loaded image doesn't have an alpha value, but it must if it wishes
         * to be compatible with the engine. Boolean return reports success of conversion. */
        virtual bool ConvertRGBToRGBA(unsigned char alpha) = 0;
        virtual bool ConvertRGBAToRGB() = 0;

        /* Returns a char array that contains the pixel data of the image.*/
        const unsigned char* GetPixelData() { return pixelData; }

        unsigned int GetWidth() { return width; } // Returns width of image
        unsigned int GetHeight() { return height; } // Returns height of image

    };

}

}

#endif
