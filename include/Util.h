/*
 * File:   Util.h
 * Author: Donald "Datriot" Whyte
 *
 * Created on January 17, 2009, 6:26 PM
 */

#ifndef UTIL_H
#define UTIL_H

#include <sstream>
#include <iostream>
#include <windows.h>

namespace parcel
{

/* Contains miscelenious functions, structs, variables and so on that are
 * used for a variety of tasks. */
namespace general
{

    /* Holds a start index and the amount of elements in an array. Can be used
     * to access a specific area of a container. */
    struct ArrayIndices
    {
        unsigned int start, amount;
    };

    /* Gets elapsed time (in milliseconds) since the last time this function was
     * called. If the parameter, 'showFPS' is true, then the Frames-per-Second
     * is printed off. */
    namespace {
        double GetElapsedTime(bool showFPS)
        {
            static long last = GetTickCount(); // Time the last fps was displayed
            static long elast = GetTickCount(); // Time this function was last called this
            static long fps = 0; // How many frames we've counted since we displayed the fps
            double etime; // The returned elapsed time since the last frame

            // Increment the frame counter
            fps++;
            // If it has been over 1000 ms (1 second) since the last frame display, display again
            if (GetTickCount() - last >= 1000)
            {
                // If flag is true, print FPS
                if (showFPS) std::cout << "FPS: " << fps << std::endl;
                // Resets counter FPS counter to 0 and last displayed time to the current time
                fps = 0;
                last = GetTickCount();
            }
            // Calculate time since this function was last called, last frame
            etime = GetTickCount() - elast;
            // Reset our elapsed time to the current time
            elast = GetTickCount();
            // Finally, return the elapsed time
            return etime;
        }
    }

    /* This function converts a certain type to a string by using a string stream.
     * It then converts the string stream into a string and returns it. */
    template<typename T> std::string ToString(T t)
    {
        std::stringstream stream;
        stream << t;
        return stream.str();
    }

    /* This function converts an std::string to an integer with the use of the
     * std::stringstream class. When using the standard std::string, the only
     * tempalte argument you have to provide is 'RT', which should be int,
     * float or double. An example call would be ToNumber<int>(str). */
    template<typename RT, typename T, typename Trait, typename Alloc>
    RT ToNumber(const std::basic_string<T, Trait, Alloc>& str)
    {
        std::basic_istringstream<T, Trait, Alloc> tempStream(str);
        RT number;
        tempStream >> number;
        return number;
    }


}

}

#endif
