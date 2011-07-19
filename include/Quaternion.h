/*
 * File:   Quaternion.h
 * Author: Donald "Datriot" Whyte
 *
 * Created on January 26, 2009, 8:46 PM
 */

#ifndef QUATERNION_H
#define QUATERNION_H

#include "Matrix.h"
#include "MCommon.h"

namespace parcel
{

namespace maths
{

    /* Quaternion class. Provides basic fuctionality so it can represent a rotaion with a smaller
     * amount of stored values (less memory consumption). NOTE: Template type must be a scalar value. */
    template<typename T>
    class Quaternion
    {


    public:

        // Components of quaternion. Can be accessed as array
        union
        {
            struct
            {
                T x, y, z, w;
            };

            T values[4];
        };



        /* Constructors. */
        Quaternion<T>() : x(0), y(0), z(0), w(1) {}

        Quaternion<T>(const T& ix, const T& iy, const T& iz, const T& iw) // Sets initial values
            : x(ix), y(iy), z(iz), w(iw) {}

        Quaternion<T>(T angle, const Vector3<T>& axis)
            : x(0), y(0), z(0), w(1) // Initialises variables to prevent errors
        {
            // Rotates given axis by given amount
            FromAxisAngle(angle, axis.x, axis.y, axis.z);
        }

        /* This function resets the quaternion so it represents no rotation. (Identity) */
        void ToIdentity()
        {
            x = y = z = 0;
            w = 1;
        }



        /* Sets the quaternion as a rotation around the specified axis. */
        void FromAxisAngle(const T& degrees, const T& axisX, const T& axisY, const T& axisZ)
        {
            // Makes sure given valuesmake up a unit vector
            if (!Vector3<T>(axisX, axisY, axisZ).IsUnit()) return;

            // Converts the angle from degrees to radians and halves it in the process
            T halfAngle = ((DegreesToRadians(degrees)) / 2);
            T sinA = (T)sin(halfAngle);

            // Calculates how much we need to rotate the axis
            w = (T)cos(halfAngle);
            // Calculates the axis to be rotated
            x = (axisX * sinA);
            y = (axisY * sinA);
            z = (axisZ * sinA);
        }
        void FromAxisAngle(const T& degrees, const Vector3<T>& axis)
        {
            // Calls true method
            FromAxisAngle(degrees, axis.x, axis.y, axis.z);
        }



        /* Returns the conjugate version of this quaternion. */
        inline Quaternion<T> GetConjugate() const
        {
            return Quaternion<T>(-x, -y, -z, w);
        }

        /* Returns the length/mangitude of the quaternion. */
        inline T Length() const
        {
            return sqrt(w * w + x * x + y * y + z * z);
        }

        inline Quaternion<T>& Normalise()
        {
            // Used to prevent any divide-by-zero problems
            T oneOverMag = 1.0f / Length();
            // Normalizes each component, giving the quaternion the length of 1
            x *= oneOverMag;
            y *= oneOverMag;
            z *= oneOverMag;
            w *= oneOverMag;

            return *this;
        }

        /* Returns dot product of this quaternion with another. */
        inline T Dot(const Quaternion<T>& quat) const
        {
            return (w * quat.w + x * quat.x + y * quat.y + z * quat.z);
        }
        /* Static method that returns dot product of two quaternions. */
        inline static T Dot(const Quaternion<T>& a, const Quaternion<T>& b)
        {
            return (a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z);
        }




        /* Converts the quaternion into a 4x4 rotation matrix. */
        inline Matrix<T> ToRotationMatrix() const
        {
            // Array that will hold the matrix's elements
            T array[16];

            // Calculates all multiples used in matrix conversation, to reduce/simplify the calculations
            const T xx = (x * x), yy = (y * y), zz = (z * z), ww = (w * w);
            const T xy = (x * y), xz = (x * z);
            const T yz = (y * z);
            const T wx = (w * x), wy = (w * y), wz = (w * z);

            // First column/vector
            array[0] = ww + xx - yy - zz;
            array[4] = 2.0 * (xy + wz);
            array[8] = 2.0 * (xz - wy);
            array[12] = 0.0;
            // Second column/vector
            array[1] = 2.0 * (xy - wz);
            array[5] = ww - xx + yy - zz;
            array[9] = 2.0 * (yz + wx);
            array[13] = 0.0;
            // Third column/vector
            array[2] = 2.0 * (xz + wy);
            array[6] = 2.0 * (yz - wx);
            array[10] = ww - xx - yy + zz;
            array[14] = 0.0;
            // Fourth column/vector. Same regardless of unit quaternion
            array[3] = 0.0;
            array[7] = 0.0;
            array[11] = 0.0;
            array[15] = 1.0;

            // Returns a freshly created matrix with calculated elements
            return Matrix<T>(4, 4, array);
        }

        inline void FromMatrix(const Matrix<T>& mat)
        {
            // Converts the given matrix into a quaternion
            T s = 0.0;
            T trace = mat(0, 0) + mat(1, 1) + mat(2, 2);

            if (trace > 0.0f)
            {
                s = sqrt(trace + 1.0);
                values[3] = s * 0.5;
                s = 0.5 / s;

                values[0] = (mat(1, 2) - mat(2, 1)) * s;
                values[1] = (mat(2, 0) - mat(0, 2)) * s;
                values[2] = (mat(0, 1) - mat(1, 0)) * s;
            }
            else
            {
                int next[3] = { 1, 2, 0 };
                int i = 0, j = 0, k = 0;

                if (mat(1, 1) > mat(0, 0))
                    i = 1;
                if (mat(2, 2) > mat(i, i))
                    i = 2;

                j = next[i];
                k = next[j];
                s = sqrt((mat(i, i) - (mat(j, j) + mat(k, k))) + 1.0);

                values[i] = s * 0.5;
                s = 0.5 / s;
                values[3] = (mat(j, k) - mat(k, j)) * s;
                values[j] = (mat(i, j) + mat(j, i)) * s;
                values[k] = (mat(i, k) + mat(k, i)) * s;
            }
        }

        inline void FromYawPitchRoll(float yaw, float pitch, float roll)
        {
            // Converts given degrees to radians
            yaw = DegreesToRadians(yaw);
            pitch = DegreesToRadians(pitch);
            roll = DegreesToRadians(roll);
            // Constructors three quaternions representing yaw, pitch and roll rotations respectively
            Quaternion<T> y, p, r;
            y = Quaternion<T>(0, sin(-yaw / 2), 0, cos(-yaw / 2));
            p = Quaternion<T>(sin(-pitch / 2), 0, 0, cos(-pitch / 2));
            r = Quaternion<T>(0, 0, sin(-roll / 2), cos(-roll / 2));
            // Concatenates the three rotations into one quaternion then normalises it
            *this = (y * p * r);

            // NOTE: May not work...
        }

        /* This method performs Spherical Linear Interpolation (SLERP) on the
         * two given quaternions, 'a' and 'b', using the given theta value, 't'
         * Using this, you can smoothly interpolate between two orientations. */
        static inline Quaternion<T> Slerp(const Quaternion<T>& a, const Quaternion<T>& b, T t)
        {
            // Calculates angle betwee nthe two quaternions
            float w1, w2;
            float cosTheta = a.Dot(b);
            float theta = (float)acos(cosTheta);
            float sinTheta = (float)sin(theta);

            // If a and b are NOT equal
            if (sinTheta > 0.001f)
            {
                w1 = static_cast<float>(sin((1.0f - t) * theta) / sinTheta);
                w2 = static_cast<float>(sin(t * theta) / sinTheta);
            }
            // If a and b are approximately equal
            else
            {
                w1 = 1.0f - t;
                w2 = t;
            }

            // Return the interpokated quaternion
            return (a * w1 + b * w2);
        }

        /* Member function version of the Slerp method. This simply passes
         * given parameters to the static method, Slerp(a, b, t). */
        inline Quaternion<T> Slerp(const Quaternion<T>& b, T t)
        {
            return Slerp(*this, b, t);
        }

        /* This method performs Normalised Linear Interpolation (NLERP) on the
         * two given quaternions. Same inputs as Slerp, with 'w2' acting as 't'.
         * This is a faster and communative alternative to Slerp. That said,
         * Slerp still has its uses. */
        static inline Quaternion<T> Nlerp(const Quaternion<T>& a, const Quaternion<T>& b, T w2)
        {
            T w1 = 1 - w2;

            return (a * w1 + b * w2).Normalise();
        }

        /* Member function version of the Nlerp method. */
        inline Quaternion<T> Nlerp(const Quaternion<T>& b, T t)
        {
            return Nlerp(*this, b, t);
        }



        /* Quaternion multiplication methods. */
        inline Quaternion<T> operator*(const Quaternion<T>& b)
        {
            // Defines two quaternions; a = this quaternion and c = result
            Quaternion<T>& a = *this;
            Quaternion<T> c;

            // Performs quaternion multiplication formula
            c.x = (a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y);
            c.y = (a.w * b.y + a.y * b.w + a.z * b.x - a.x * b.z);
            c.z = (a.w * b.z + a.z * b.w + a.x * b.y - a.y * b.x);
            c.w = (a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z);

            // Assigns result in self and returns value of itself
            return c;

        }
        inline Quaternion<T>& operator*=(const Quaternion<T>& quat)
        {
            // Multiplies self by given quaternion and returns itself
            (*this) = (*this) * quat;
            return *this;
        }



        /* Scalar multiplication. Times each component by the given value. */
        inline Quaternion<T> operator*(const T& s) const
        {
            return Quaternion<T>(x * s, y * s, z * s, w * s);
        }



        /* Multiples this quaternion by a vector and returns the resultant vector. */
        inline Vector3<T> operator*(const Vector3<T>& vec)
        {
            // Normalises the given vector, making it a unit vector
            Vector3<T> vn = vec;
            vn.Normalise();

            Quaternion<T> vectorQuat, resultQuat;

            // Stores the unit vector in the vectorQuaternion
            vectorQuat.x = vn.x;
            vectorQuat.y = vn.y;
            vectorQuat.z = vn.z;
            vectorQuat.w = 0.0;
            // Multiplying the vector quaternion by the conjugate of this one
            resultQuat = (((*this) * vectorQuat) * GetConjugate());

            // Returns a vector containing the result's values
            return Vector3<T>(resultQuat.x, resultQuat.y, resultQuat.z);
        }



        /* Quaternion addition. Adds each component with the given quaternion's components. */
        inline Quaternion<T> operator+(const Quaternion<T>& quat)
        {
            return Quaternion<T>(x + quat.x, y + quat.y, z + quat.z, w + quat.w);
        }
        inline Quaternion<T> operator+=(const Quaternion<T>& quat)
        {
            // Adds self to the given quaternion and returns itself
            (*this) = (*this) + quat;
            return *this;
        }



        /* Three boolean checkers. One checks if the quaternion is equal to the
         * given one, one checks if it is an identity quaternion and the other
         * checks if it is a unit (length is one) quaternion. */
        inline bool operator==(const Quaternion<T>& quat) const
        {
            return (x == quat.x && y == quat.y && z == quat.z && w == quat.w);
        }
        inline bool IsIdentity() const
        {
            return (x == 0.0 && y == 0.0 && z == 0.0 && w = 1.0);
        }
        inline bool IsUnit() const
        {
            T mag = Length(); // Gets length
            /* Checks if is quaternion's maginitude is close enough to 1.
             * This is to prevent flaoting point drift from causing errors. */
            return CloseEnough(mag, 1.0f);
        }


    };

    typedef Quaternion<int> quaternioni;
    typedef Quaternion<float> quaternionf;
    typedef Quaternion<double> quaterniond;

}

}

#endif
