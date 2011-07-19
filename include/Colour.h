/*
 * File:   Colour.h
 * Author: Donald
 *
 * Created on December 22, 2008, 7:56 PM
 */

#ifndef COLOUR_H
#define COLOUR_H

#include <cmath>

namespace parcel
{

namespace graphics
{

    /* Simple colour class that holds red, green, blue and alpha components.
     * It has operators so you can add, subtract, multiply and divide colours
     * together. You can even check if two colours are the same or not.
     * The ClampValues method keeps all values of the colour at a maximum
     * of 1.0f. Graphics libraries, such as OpenGL only support a maximum of
     * 1.0f for each colour component, so it makes sense to do this. */
    template<typename T>
    class Colour
    {

    public:

        /* Fields. The red, blue, green and alpha compnents of the colour.
         * Can be accessed in array format too. */
        union
        {
            struct
            {
                T red, green, blue, alpha;
            };
            struct
            {
                T r, g, b, a;
            };
            T values[4];
        };

        /* Constructors, one gives variables a default value while other initializes them with specific values. */
        Colour() {}
        Colour(T r, T g, T b, T a) : red(r), green(g), blue(b), alpha(a) {}

        Colour<T> operator+(const Colour<T>& c) const // Addition
        {
            return Colour((r + c.r), (g + c.g), (b + c.b), (a + c.a));
        }
        Colour<T> operator-(const Colour<T>& c) const // Subtraction
        {
            return Colour((r - c.r), (g - c.g), (b - c.b), (a - c.a));
        }
        Colour<T> operator*(const Colour<T>& c) const // Multiplication
        {
            return Colour((r * c.r), (g * c.g), (b * c.b), (a * c.a));
        }
        Colour<T> operator/(const Colour<T>& c) const // Division
        {
            return Colour((r / c.r), (g / c.g), (b / c.b), (a / c.a));
        }

        Colour<T>& operator=(const Colour<T>& c) // Assignment operator
        {
            // Assigns all values from given colour to this one
            r = c.r;
            g = c.g;
            b = c.b;
            a = c.a;

            return *this; // Returns reference to this instance
        }
        Colour<T>& operator+=(const Colour<T> &c) // Assignment addition
        {
            *this = *this + c;
            return *this;
        }
        Colour<T>& operator-=(const Colour<T> &c) // Assignment subtraction
        {
            *this = *this - c;
            return *this;
        }
        Colour<T>& operator*=(const Colour<T> &c) // Assignment multiplcation
        {
            *this = *this * c;
            return *this;
        }
        Colour<T>& operator/=(const Colour<T> &c) // Assigment division
        {
            *this = *this / c;
            return *this;
        }

        bool operator==(const Colour<T> &c) const // Equality operator, are the colours the same?
        {
            // Checks each colour component is the same and returns the result
            return (r == c.r && g == c.g && b == c.b && a == c.a);
        }
        bool operator!=(const Colour<T> &c) const // Inequality operator, are the colours NOT the same?
        {
            // Checks each colour component is NOT the same and returns the result
            return (r != c.r && g != c.g && b != c.b && a != c.a);
        }

    };

    // We use template specialization for Colour<float> because we may need to clamp the values to 1.0f
    template<>
    class Colour <float>
    {

    public:

        union
        {
            struct
            {
                float red, green, blue, alpha;
            };
            struct
            {
                float r, g, b, a;
            };
            float values[4];
        };

        Colour() {}
        Colour(float r, float g, float b, float a) : red(r), green(g), blue(b), alpha(a) {}

        Colour<float> operator+(const Colour<float>& c) const
        {
            return Colour((r + c.r), (g + c.g), (b + c.b), (a + c.a));
        }
        Colour<float> operator-(const Colour<float>& c) const
        {
            return Colour((r - c.r), (g - c.g), (b - c.b), (a - c.a));
        }
        Colour<float> operator*(const Colour<float>& c) const
        {
            return Colour((r * c.r), (g * c.g), (b * c.b), (a * c.a));
        }
        Colour<float> operator/(const Colour<float>& c) const
        {
            return Colour((r / c.r), (g / c.g), (b / c.b), (a / c.a));
        }

        Colour<float>& operator=(const Colour<float>& c)
        {
            r = c.r;
            g = c.g;
            b = c.b;
            a = c.a;

            return *this;
        }
        Colour<float>& operator+=(const Colour<float> &c)
        {
            *this = *this + c;
            return *this;
        }
        Colour<float>& operator-=(const Colour<float> &c)
        {
            *this = *this - c;
            return *this;
        }
        Colour<float>& operator*=(const Colour<float> &c)
        {
            *this = *this * c;
            return *this;
        }
        Colour<float>& operator/=(const Colour<float> &c)
        {
            *this = *this / c;
            return *this;
        }

        bool operator==(const Colour<float> &c) const
        {
            return (r == c.r && g == c.g && b == c.b && a == c.a);
        }
        bool operator!=(const Colour<float> &c) const
        {
            return (r != c.r && g != c.g && b != c.b && a != c.a);
        }

        /* Checks if any of the colour's values have gone over 1. If they have, clamp them by
         * giving them a value of 1. */
        inline void ClampValues()
        {
            if (fabs(r) > 1.0f) r = 1.0f;
            if (fabs(g) > 1.0f) g = 1.0f;
            if (fabs(b) > 1.0f) b = 1.0f;
            if (fabs(a) > 1.0f) a = 1.0f;
        }
    };


    // Some typedefs to save me a bit of time
    typedef Colour<int> colouri;
    typedef Colour<float> colourf;
    typedef Colour<double> colourd;

    // A few basic colours are defined here to save me a bit of time
    namespace presetcolours
    {

        static const colourf Black = colourf(0.0f, 0.0f, 0.0f, 1.0f);
        static const colourf Gray = colourf(0.5f, 0.5f, 0.5f, 1.0f);
        static const colourf White = colourf(1.0f, 1.0f, 1.0f, 1.0f);
        static const colourf Red = colourf(1.0f, 0.0f, 0.0f, 1.0f);
        static const colourf Green = colourf(0.0f, 1.0f, 0.0f, 1.0f);
        static const colourf Blue = colourf(0.0f, 0.0f, 1.0f, 1.0f);

    }

}

}

#endif /* COLOUR_H */
