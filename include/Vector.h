/*
 * File:   Vector.h
 * Author: Donald
 *
 * Created on September 4, 2008, 4:08 PM
 */

#ifndef VECTOR_H
#define	VECTOR_H

#include <typeinfo>
#include <math.h>

namespace parcel
{

namespace maths
{

    template<typename T>
    class Vector2
    {

        /* Fields. */
    public:

        // Union is used so values can be accessed with standard x, y, z, w, etc. and can be accessed with array.
        union
        {
            struct
            {
                T x;
                T y;
            };

            T values[2];
        };

        typedef Vector2<T> vector2t; // Used for easier typing

        /* Constructors. */
        Vector2() {}
        // Copy constructor
        Vector2(const vector2t &a) :
			x(a.x), y(a.y) {}
        // Definition constructor
        Vector2(T _x, T _y) :
			x(_x), y(_y) {}

        /* Operator overloads */

        /* Assignment operator */
        vector2t &operator =(const vector2t &a)
        {
            x = a.x; y = a.y;
            return *this;
        }

        /* Equality operator */
        bool operator ==(const vector2t &a) const
        {
            return (x == a.x) && (y == a.y);
        }

        /* Not equal operator */
        bool operator !=(const vector2t &a) const
        {
             return (x != a.x) || (y != a.y);
        }

        /* Vector addition operation */
        inline vector2t operator +(const vector2t &a)
        {
            return vector2t((x + a.x), (y + a.y));
        }

        /* Vector subtraction operation */
        inline vector2t operator -(const vector2t &a)
        {
            return vector2t((x - a.x), (y - a.y));
        }

        /* Vector multiplication operation */
        inline vector2t operator *(T a)
        {
            return vector2t((x * a), (y * a));
        }

        /* Dot product of the vector */
        inline vector2t operator *(const vector2t &a) const
        {
            return vector2t((x * a.x), (y * a.y));
        }

        /* Vector division operation */
        inline vector2t operator /(T a)
        {
            T value = (1.0f / a);
            return vector2t((x * value), (y * value));
        }

        /* Combined assignment operators */

        /* Permenent vector addition */
        vector2t &operator +=(const vector2t &a)
        {
            x += (a.x); y += (a.y);
            return *this;
        }

        /* Permenent vector subtraction operation */
        vector2t &operator -=(const vector2t &a)
        {
            x -= (a.x); y -= (a.y);
            return *this;
        }

        /* Permenent vector multiplation operation */
        vector2t &operator *=(const vector2t &a)
        {
            x *= (a.x); y *= (a.y);
            return *this;
        }

        /* Permenent vector division operation */
        vector2t &operator /=(T a)
        {
            T value = (1.0f / a);
            x /= value;; y /= value;
            return *this;
        }

        /* The negative value of the vector */
        vector2t operator -() const { return vector2t(-x, -y); }


        /* Methods */

        /* Changes the vector to a zero vector */
        inline vector2t& Zero() { x = y = 0.0f; return *this; }
        /* Length of vector. */
        inline T Length() const { return (T)sqrt(x * x + y * y); }
        /* Squared length of vector. */
        inline T SqrLength() { return (x * x + y * y); }
        /* Dot product. */
        inline T Dot(const vector2t &a)
		{ return (x * a.x + y * a.y); }

        /* Normalizes the vector */
        inline Vector2<T>& Normalise()
        {
			//  Normalizes the values
			T oneOverMag = 1.0f / Length();
			x *= oneOverMag;
			y *= oneOverMag;

			return *this;
        }

        /* Returns true if this is a unit (normalised) vector. */
        inline bool IsUnit()
        {
            T mag = Length(); // Gets length

            // If it is a floating point number, use fabs for accurate checking
            if (typeid(mag) == typeid(float))
                return (fabs((float)mag) == 1.0f);
            // Otherwise, just do a standard check
            else
                return (mag == 1.0);
        }

        /* Static method. This converts a group of vectors into an array and returns that array.
         * NOTE: Returns dynamic array. This means the returned array has to be cleaned up
         * by using delete[]. Work on a way to change this. */
        static inline T* ConvertToArray(Vector2<T>* vectors, unsigned int numVectors)
        {
            unsigned int size = (numVectors * 2); // Size of the array
            T* array = new float[size]; // Dynamic allocation

            unsigned int i, j;
            for (i = j = 0; (i < numVectors); i++)
            {
                array[j] = vectors[i].x; j++;
                array[j] = vectors[i].y; j++;
            }

            return array;
        }
        /* Static method that returns the dot product of two vectors.*/
        static inline T Dot(const vector2t &a1, const vector2t &a2)
		{ return (a1.x * a2.x + a1.y * a2.y); }



        // NOTE: THEY CAUSE AMBIGIOUS OVERLOADS!
//        /* External friend functions for +, -, * and /. */
//        friend inline vector2t operator+(const vector2t a, const vector2t& b)
//        {
//            return vector2t(a.x + b.x, a.y + b.y);
//        }
//        friend inline vector2t operator-(const vector2t& a, const vector2t& b)
//        {
//            return vector2t(a.x - b.x, a.y - b.y);
//        }
//        friend inline vector2t operator*(const vector2t& a, const vector2t& b)
//        {
//            return vector2t(a.x * b.x, a.y * b.y);
//        }
//        friend inline vector2t operator/(const vector2t& a, const vector2t& b)
//        {
//            return vector2t(a.x / b.x, a.y / b.y);
//        }


    };

    template<typename T>
    class Vector3
    {

        /* Fields. */
    public:

        union
        {
            struct
            {
                T x;
                T y;
                T z;
            };

            T values[3];
        };

        typedef Vector3<T> vector3t; // Used so code is less bloated

        /* Constructors. */
        Vector3() {}
        // Copy constructor
        Vector3(const vector3t &a) :
			x(a.x), y(a.y), z(a.z) {}
        // Definition constructor
        Vector3(T _x, T _y, T _z) :
			x(_x), y(_y), z(_z) {}

        /* Operator overloads */

        /* Assignment operator */
        vector3t &operator =(const vector3t &a)
        {
            x = a.x; y = a.y; z = a.z;
            return *this;
        }

        /* Equality operator */
        bool operator ==(const vector3t &a) const
        {
            return (x == a.x) && (y == a.y) && (z == a.z);
        }

        /* Not equal operator */
        bool operator !=(const vector3t &a) const
        {
             return (x != a.x) || (y != a.y) || (z != a.z);
        }

        /* Vector addition operation */
        inline vector3t operator +(const vector3t &a)
        {
            return vector3t((x + a.x), (y + a.y), (z + a.z));
        }

        /* Vector subtraction operation */
        inline vector3t operator -(const vector3t &a)
        {
            return vector3t((x - a.x), (y - a.y), (z - a.z));
        }

        /* Vector multiplication operation */
        inline vector3t operator *(T a)
        {
            return vector3t((x * a), (y * a), (z * a));
        }

        /* Mutliplication of vector with another vector. Returns the sum. */
        inline vector3t operator *(const vector3t &a) const
        {
            return vector3t((x * a.x), (y * a.y), (z * a.z));
        }

        /* Vector division operation */
        inline vector3t operator /(T a)
        {
            T value = (1.0f / a); // Prevents divide-by-zero error
            return vector3t((x * value), (y * value), (z * value));
        }

        /* Combined assignment operators */

        /* Permenent vector addition */
        vector3t &operator +=(const vector3t &a)
        {
            x += (a.x); y += (a.y); z += (a.z);
            return *this;
        }

        /* Permenent vector subtraction operation */
        vector3t &operator -=(const vector3t &a)
        {
            x -= (a.x); y -= (a.y); z -= (a.z);
            return *this;
        }

        /* Permenent vector multiplation operation */
        vector3t &operator *=(const vector3t &a)
        {
            x *= (a.x); y *= (a.y); z *= (a.z);
            return *this;
        }

        /* Permenent vector division operation */
        vector3t &operator /=(T a)
        {
            T value = (1.0f / a); // Prevents divide-by-zero error
            x *= value; y *= value; z *= value;
            return *this;
        }

        /* The negative value of the vector */
        vector3t operator -() const { return vector3t(-x, -y, -z); }


        /* Methods */

        /* Changes the vector to a zero vector */
        inline vector3t& Zero() { x = y = z = 0.0f; return *this; }
        /* Length of vector. */
        inline T Length() const { return (T)sqrt(x * x + y * y + z * z); }
        /* Squared length of vector. */
        inline T SqrLength() { return (x * x + y * y + z * z); }
        /* Dot product. */
        inline T Dot(const vector3t &a)
		{ return (x * a.x + y * a.y + z * a.z); }

        /* Normalizes the vector */
        inline Vector3<T>& Normalise()
        {
			//  Normalizes the values
			T oneOverMag = 1.0f / Length();
			x *= oneOverMag;
			y *= oneOverMag;
			z *= oneOverMag;

			return *this;
        }

        /* Returns true if this is a unit (normalised) vector. */
        inline bool IsUnit()
        {
            T mag = Length(); // Gets length

            // If it is a floating point number, use fabs for accurate checking
            if (typeid(mag) == typeid(float))
                return (fabs((float)mag) == 1.0f);
            // Otherwise, just do a standard check
            else
                return (mag == 1.0);
        }

        /* Static method. This converts a group of vectors into an array and returns that array.
         * NOTE: Returns dynamic array. This means the returned array has to be cleaned up
         * by using delete[]. Work on a way to change this. */
        static inline T* ConvertToArray(Vector3<T>* vectors, unsigned int numVectors)
        {
            unsigned int size = (numVectors * 3); // Size of the array
            T* array = new float[size]; // Dynamic allocation

            unsigned int i, j;
            for (i = j = 0; (i < numVectors); i++)
            {
                array[j] = vectors[i].x; j++;
                array[j] = vectors[i].y; j++;
                array[j] = vectors[i].z; j++;
            }

            return array;
        }
        /* Static method that returns the dot product of two vectors.*/
        static inline T Dot(const vector3t &a1, const vector3t &a2)
		{ return (a1.x * a2.x + a1.y * a2.y + a1.z * a2.z); }



        /* External friend functions for +, -, * and /. */
//        friend inline vector3t operator+(const vector3t& a, const vector3t& b)
//        {
//            return vector3t(a.x + b.x, a.y + b.y, a.z + b.z);
//        }
//        friend inline vector3t operator-(const vector3t& a, const vector3t& b)
//        {
//            return vector3t(a.x - b.x, a.y - b.y, a.z - b.z);
//        }
//        friend inline vector3t operator*(const vector3t& a, const vector3t& b)
//        {
//            return vector3t(a.x * b.x, a.y * b.y, a.z * b.z);
//        }
//        friend inline vector3t operator/(const vector3t& a, const vector3t& b)
//        {
//            return vector3t(a.x / b.x, a.y / b.y, a.z / b.z);
//        }


        /* Vector3-specific operations. */

        /* Cross product. */
        inline vector3t Cross(const vector3t &a)
        { return vector3t(y * a.z - z * a.y, z * a.x - x * a.z, x * a.y - y * a.x); }

        /* Static method that returns cross product of two vectors. */
        static inline vector3t Cross(const vector3t &a1, const vector3t &a2)
        { return vector3t(a1.y * a2.z - a1.z * a2.y, a1.z * a2.x - a1.x * a2.z, a1.x * a2.y - a1.y * a2.x); }


    };

    template<typename T>
    class Vector4
    {

        /* Fields. */
    public:
        union
        {
            struct
            {
                T x;
                T y;
                T z;
                T w;
            };

            T values[4];
        };

        typedef Vector4<T> vector4t; // Used for easier typing


        /* Constructors. */
        Vector4() {}
        // Copy constructor
        Vector4(const vector4t &a) :
			x(a.x), y(a.y), z(a.z), w(a.w) {}
        // Definition constructor
        Vector4(T _x, T _y, T _z, T _w) :
			x(_x), y(_y), z(_z), w(_w) {}

        /* Operator overloads */

        /* Assignment operator */
        vector4t &operator=(const vector4t &a)
        {
            x = a.x; y = a.y; z = a.z; w = a.w;
            return *this;
        }

        /* Equality operator */
        bool operator ==(const vector4t &a) const
        {
            return (x == a.x) && (y == a.y) && (z == a.z) && (w == a.w);
        }

        /* Not equal operator */
        bool operator !=(const vector4t &a) const
        {
             return (x != a.x) || (y != a.y) || (z != a.z) || (w != a.w);
        }

        /* Vector addition operation */
        inline vector4t operator +(const vector4t &a)
        {
            return vector4t((x + a.x), (y + a.y), (z + a.z), (w + a.w));
        }

        /* Vector subtraction operation */
        inline vector4t operator -(const vector4t &a)
        {
            return vector4t((x - a.x), (y - a.y), (z - a.z), (w - a.w));
        }

        /* Vector multiplication operation */
        inline vector4t operator *(T a)
        {
            return vector4t((x * a), (y * a), (z * a), (w * a));
        }

        /* Mutliplication of vector with another vector. Returns the sum. */
        inline vector4t operator *(const vector4t &a) const
        {
            return vector4t((x * a.x), (y * a.y), (z * a.z), (w * a.w));
        }


        /* Vector division operation */
        inline vector4t operator /(T a)
        {
            T value = (1.0f / a);
            return vector4t((x * value), (y * value), (z * value), (w * value));
        }

        /* Combined assignment operators */

        /* Permenent vector addition */
        vector4t &operator +=(const vector4t &a)
        {
            x += (a.x); y += (a.y); z += (a.z); w += (a.w);
            return *this;
        }

        /* Permenent vector subtraction operation */
        vector4t &operator -=(const vector4t &a)
        {
            x -= (a.x); y -= (a.y); z -= (a.z);
            return *this;
        }

        /* Permenent vector multiplation operation */
        vector4t &operator *=(const vector4t &a)
        {
            x *= (a.x); y *= (a.y); z *= (a.z); w *= (a.w);
            return *this;
        }

        /* Permenent vector division operation */
        vector4t &operator /=(T a)
        {
            T value = (1.0f / a);
            x /= value;; y /= value; z /= value; w /= value;
            return *this;
        }

        /* The negative value of the vector */
        vector4t operator -() const { return vector4t(-x, -y, -z, -w); }


        /* Methods */

        /* Changes the vector to a zero vector */
        inline vector4t Zero() { x = y = z = w = 0.0f; return *this; }
        /* Length of vector. */
        inline T Length() const { return (T)sqrt(x * x + y * y + z * z + w * w); }
        /* Squared length of vector. */
        inline T SqrLength() { return (x * x + y * y + z * z + w * w); }
        /* Dot product. */
        inline T Dot(const vector4t &a)
		{ return (x * a.x + y * a.y + z * z * a.z + w * a.w); }

        /* Normalizes the vector */
        inline Vector4<T>& Normalise()
        {
			T oneOverMag = 1.0f / Length();
			x *= oneOverMag;
			y *= oneOverMag;
			z *= oneOverMag;
			w *= oneOverMag;

			return *this;
        }

        /* Returns true if this is a unit (normalised) vector. */
        inline bool IsUnit()
        {
            T mag = Length(); // Gets length

            // If it is a floating point number, use fabs for accurate checking
            if (typeid(mag) == typeid(float))
                return (fabs((float)mag) == 1.0f);
            // Otherwise, just do a standard check
            else
                return (mag == 1.0);
        }

        /* Static method. This converts a group of vectors into an array and returns that array.
         * NOTE: Returns dynamic array. This means the returned array has to be cleaned up
         * by using delete[]. Work on a way to change this. */
        static inline T* ConvertToArray(Vector4<T>* vectors, unsigned int numVectors)
        {
            unsigned int size = (numVectors * 4); // Size of the array
            T* array = new float[size]; // Dynamic allocation

            unsigned int i, j;
            for (i = j = 0; (i < numVectors); i++)
            {
                array[j] = vectors[i].x; j++;
                array[j] = vectors[i].y; j++;
                array[j] = vectors[i].z; j++;
                array[j] = vectors[i].w; j++;
            }

            return array;
        }
        /* Static method that returns the dot product of two vectors.*/
        static inline T Dot(const vector4t &a1, const vector4t &a2)
		{ return (a1.x * a2.x + a1.y * a2.y + a1.z * a2.z + a1.w * a2.w); }



        /* External friend functions for +, -, * and /. */
//        friend inline vector4t operator+(const vector4t& a, const vector4t& b)
//        {
//            return vector4t(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
//        }
//        friend inline vector4t operator-(const vector4t& a, const vector4t& b)
//        {
//            return vector4t(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
//        }
//        friend inline vector4t operator*(const vector4t& a, const vector4t& b)
//        {
//            return vector4t(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
//        }
//        friend inline vector4t operator/(const vector4t& a, const vector4t& b)
//        {
//            return vector4t(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
//        }


    };


	// A few typedefs used for easier access to certain types of vectors
	typedef Vector2<int> vector2i;
	typedef Vector3<int> vector3i;
	typedef Vector4<int> vector4i;

	typedef Vector2<float> vector2f;
	typedef Vector3<float> vector3f;
	typedef Vector4<float> vector4f;

    typedef Vector2<double> vector2d;
	typedef Vector3<double> vector3d;
	typedef Vector4<double> vector4d;


}

}

#endif	/* _VECTOR_H */
