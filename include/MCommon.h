/*
 * File:   MCommon.h
 * Author: Donald "Datriot" Whyte
 *
 * Created on January 21, 2009, 2:30 PM
 */

/* This contains common variables or methods. Also <cmath> and other
 * functions are include in here, so instead of including those in other
 * math headers, you can just include "MCommon.h". */

#ifndef MCOMMON_H
#define MCOMMON_H

#include <math.h>

namespace parcel
{

namespace maths
{

    namespace constants
    {

        static const double PI = 3.14159265358979323846;
        // Value for a right angle in both degrees and radians
        static const double rightAngleInDegrees = 90;
        static const double rightAngleInRadians = 1.5708;
        // Used as the threshhold for checking floating point equality
        static const double epsilon = 1e-6f;


    }

    /* Converts degrees and radians and radians to degrees respectively. */
    namespace
    {
        double DegreesToRadians(const double& a)
        {
            return (a * 0.01745329251994329576923690768488);
        }
    }
    namespace
    {
        double RadiansToDegrees(const double& a)
        {
            return (a * 57.2957795130823208767981548141052);
        }
    }
    /* Determines whether the two floating-point values f1 and f2 are
     * close enough together that they can be considered equal. Is used
     * combat floatig point inaccuracy. */
    namespace
    {
        bool CloseEnough(const float& f1, const float& f2)
        {
            return fabsf((f1 - f2) / ((f2 == 0.0f) ? 1.0f : f2)) < constants::epsilon;
        }
    }
    /* Converts the given integer into the nearest power-of-two integer. */
    namespace
    {
        int ClosestPow2(int value)
        {
            int newVal = 2;
            while (newVal < value)
            {
                newVal <<= 1;
            }
            return newVal;
        }
    }
    /* Rounds an integer to the nearest multiple of another number.
     * 'number' is the number that will be rounded.
     * 'significance' is the number which the result has to be a multiple of.
     * e.g. 'number' is 23. 'significance' is 10, result will be 20 (multiple of 10). */
    namespace
    {
        int Quantize(int number, int significance)
        {
            double d = (number / significance);
            d = ceil(d - 0.5); // - 0.5 used so it doesn't round up, but round to nearest
            return static_cast<int>(d * significance);
        }
    }


    /* NOTE: All of these global functions are wrapped in anonymous namespaces
     * to avoid redeclaration issues. I could have used static, but that would
     * have defined the functions in every file that includes this, meaning
     * functions would have been define for files that don't even use it. Also,
     * I believe this is the preffered way to handle the issue, since a lot of
     * people condemn the use of static functions out of class/struct scope. */


}

}

#endif
