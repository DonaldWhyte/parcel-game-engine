/*
 * File:   Matrix.h
 * Author: Donald
 *
 * Created on September 18, 2008, 5:32 PM
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <vector>
#include "Vector.h"
#include "MCommon.h"
#include "Util.h"

namespace parcel
{

namespace maths
{

	/* NOTE - Things to do:
	 * 1) Create all the methods I haven't created yet.
	 * 2) Make better vector multiplication functions, currently only supports 3x3 matrices with
	        a Vector3. Not doing it now because it'll require me to completely redesign my vector class
	        and may make it uneededly complex.
	 * References:
	 * http://darwin2k.sourceforge.net/darwin2k-docs/matrix.html#DOC.24.1.4.34
	*/

	/* This class holds all the values and operations needed for a generic matrix. Uses column-major form. */
	template<typename T>
	class Matrix
	{

		typedef std::vector< std::vector<T> > ElemArray; // Used for easier typing
		typedef std::vector<T> Row, Column; //  A row/column of an array

	private:

		/* Holds all of the matrix's elements in vector of vectors. (Lol!)
         * The array is column-major*/
		ElemArray elements;
		// The amount of rows and columns the matrix has.
		unsigned int rows, columns;

        /* Method to check if a matrix has the same dimensions as another. */
        bool SameDimensions(const Matrix &m);

	public:

		Matrix(); // Constructs an uninitialized matrix.
		Matrix(int n); // Constructs an (N x N) matrix.
		Matrix (int nr, int nc); // Constructs an (NR x NC) matrix
		/* Constructs an (NR x NC) matrix and sets its elements to the
		 * values pointed to by data. Data must be in column-major form like this:
                 * { v11, v21, v31, v41,
                 *   v12, v22, v32, v42,
                 *   v13, v23, v33, v43,
                 *   v14, v24, v35, v44 }
                 */
		Matrix(int nr, int nc, const T* data);
		Matrix(const Matrix<T> &m);	// Copy constructor.

		/* Operator overloads. */
		Matrix<T> &operator=(const Matrix<T> &m); // Assigns matrix to this matrix
		Matrix<T> &operator*=(T m); // Multiplies matrix by a scalar value
        Matrix<T> &operator*=(const Matrix<T> &m); // Multiplies matrix by another matrix
		Matrix<T> &operator+=(const Matrix<T> &m); // Adds another matrix to this one
		Matrix<T> &operator-=(const Matrix<T> &m); // Subtracts matrix with the given one
        Matrix operator -(); // Returns negative version of this matrix
		bool operator==(Matrix<T>&m); // Equality operator
		bool operator!=(Matrix<T> &m); // Not equal operator

		/* Methods. */
		Matrix<T> ToIdentity(); // Makes current matrix identity
		Matrix<T> Zero(); // Returns this matrix, but zeroed
		Matrix<T> Transpose(); // Transposes the matrix.
		const Matrix<T>& Negate(); // Negates all elements in the matrix.
		Matrix<T> Cofactor() const; // Returns a copy of the cofactor matrix
		Matrix<T> Matrix<T>::Adjoint() const; // Returns a copy of the TRANSPOSED cofactor matrix
		T Determinant() const; // Calculates and returns the determinant of this matrix
		Matrix<T> Inverse() const; // Calculates inverse of matrix and returns a copy of it
		const Matrix<T>& ToInverse(); // Inverses the matrix itself and not a copy, unlike the above method
		// Uses the given yaw, pitch and roll degrees to constructor a rotation matrix
		const Matrix<T>& FromYawPitchRoll(T yawDegrees, T pitchDegrees, T rollDegrees);
		// Converst matrix to a 4x4 rotation matrix that rotates the specified axis (x, y or z)
		const Matrix<T>& ToRotationX(T degrees);
		const Matrix<T>& ToRotationY(T degrees);
		const Matrix<T>& ToRotationZ(T degrees);
		// Creates an orthographic  projection with the given dimensions
		const Matrix<T>& ToOrthoProjection(T left, T right, T bottom, T top, T zNear, T zFar);

		/* Inline Methods. */
		const unsigned int &Rows() const; // Returns amount of rows
		const unsigned int &Columns() const; // Returns amount of columns
        const T &operator()(int r, int c) const; // Gets element at (row, column)
		const T &Element(int r, int c) const;  // Same as above
		void SetElement(const int& r, const int& c, const T& newElem); // Changes element at (r,c) to new value
		T* Address(int r, int c); // Returns a pointer to a specific element of the matrix
		T* ColumnAddress(int c); // Returns first element of the specified column array
		bool IsSquare() const; // Returns true if matrix is a sqaure, false if not
		/* Returns an array of needed type, in column-major form to use with GL functuons.
		 * NOTE: The array returned is allocated on the heap, so you have to manually delete
		 * the pointer using delete[]. */
		T* ToArray() const;
		/* Ths one doesn't create a new array on the heap, but fills a given array with the values. */
		void ToArray(T* array) const;

        /* This function prinns the number of the matrix's rows, columns and all of its elements. */
		void Print() const;
        /* This returns a column of a matrix. as a 3D vector. m is the matrix and c
         * is the number of the column requested. If the matrix is not a 3x3, 3x4,
         * 4x3 or 4x4 matrix, then it returns a blank vector. */
        Vector3<T> GetColumnAsVector3(const int& c);

		/* Static Utility Methods. Implemented in header. */

		/* Returns an (N x N) identity matrix. */
		static Matrix<T> Identity(const int &n)
		{
		    Matrix m(n);

		    // Sets diagonals to 1
            for (int i = 0; (i < n); i++) m.elements[i][i] = 1;

		    return m;
		}
		/* Checks if the two matrices given in the parameters have the same dimensions. */
		static bool SameDimensions(const Matrix<T> &m1, const Matrix<T> &m2)
        {
            if ((m1.Rows() == m2.Rows()) && (m1.Columns() == m2.Columns()))
                return true;
            else
                return false;
        }




        /* Friend functions of class Matrix. All these return a blank matrix if the two matrices aren't the same dimensions.
         * Also implemented into the header. */

		friend Matrix<T> operator+(Matrix<T> &m1, Matrix<T> &m2)  // Adds two matrices together
        {
            int rows = m1.Rows();
            int columns = m1.Columns();
            Matrix<T> m3(rows, columns); // The sum of the operation between m1 and m2

            if (Matrix<T>::SameDimensions(m1, m2))
            {
                /* Adds every element in m1 by the corresponing element in m2 and the sum
                 * of that is assigned to the corresponding element in m3. */
                for (int i = 0; (i < columns); i++)
                    for (int j = 0; (j < rows); j++)
                        m3.elements[i][j] = (m1.elements[i][j] + m2.elements[i][j]);
            }

            return m3;
        }

		friend Matrix<T> operator-(Matrix<T> &m1, Matrix<T> &m2) // Subtracts two matrices together
        {
            int rows = m1.Rows();
            int columns = m1.Columns();
            Matrix<T> m3(rows, columns); // The sum of the operation between m1 and m2

            if (Matrix<T>::SameDimensions(m1, m2))
            {
                /* Subtracts every element in m1 by the corresponing element in m2 and the sum
                 * of that is assigned to the corresponding element in m3. */
                for (int i = 0; (i < columns); i++)
                    for (int j = 0; (j < rows); j++)
                        m3.elements[i][j] = (m1.elements[i][j] - m2.elements[i][j] );
            }

            return m3;
        }

		friend Matrix<T> operator*(const double &d, const Matrix<T> &m) // Multiplies matrix with a scalar value
        {
            int rows = m.Rows();
            int columns = m.Columns();
            Matrix<T> m2(rows, columns); // Sum of scalar multiplication

            /* Multiplies every element of the given matrix by the given scalar and then
             * assigns the sum to the corresponding element in m2. */
            for (int i = 0; (i < columns); i++)
                for (int j = 0; (j < rows); j++)
                    m2.elements[i][j] = (m.elements[i][j] * d);

            return m2;
        }

		friend Matrix<T> operator*(const Matrix<T> &m, const double &d) // Same as above
        {
            int rows = m.Rows();
            int columns = m.Columns();
            Matrix<T> m2(rows, columns); // Sum of scalar multiplication

            /* Multiplies every element of the given matrix by the given scalar and then
             * assigns the sum to the corresponding element in m2. */
            for (int i = 0; (i < columns); i++)
                for (int j = 0; (j < rows); j++)
                    m2.elements[i][j] = (m.elements[i][j] * d);

            return m2;
        }

		friend Vector3<T> operator*(const Matrix<T> &m, const Vector3<T> &v) // Multiplies a matrix with a vector, returns the resultant vector
        {
            /* If this matrix's columns are NOT the size of the vector, then that means
             * that they are non-conformable and thus cannot be multiplied. We shall return
             * a default vector. =( Also, rows must be 3 as well, otherwise the size of the
             * returned vector will change, and it doesn't support this at the moment.
             * A fourth column is okay too, it will just be ignored. This is for 4x4 homogenous matrices. */
            if (m.Rows() < 3 || m.Rows() > 4 || m.Columns() < 3 || m.Columns() > 4)
            {
                return Vector3<T>().Zero();
            }

            Vector3<T> v2; // Sum of the multiplication

            /* Performs vector-matrix multiplication, can't be bothered going into detail. =p */
            v2.x = ((m(0 , 0) * v.x) + (m(0, 1) * v.y) + (m(0 , 2) * v.z));
            v2.y = ((m(1, 0) * v.x) + (m(1 , 1) * v.y) + (m(1 , 2) * v.z));
            v2.z = ((m(2, 0) * v.x) + (m(2, 1) * v.y) + (m(2 , 2) * v.z));

            return v2;
        }

		friend Vector3<T> operator*(const Vector3<T> &v, const Matrix<T> &m) // Same as above
        {
            if (m.Rows() < 3 || m.Rows() > 4 || m.Columns() < 3 || m.Columns() > 4)
            {
                return Vector3<T>();
            }

            Vector3<T> v2;

            v2.x = ((m(0 , 0) * v.x) + (m(0, 1) * v.y) + (m(0 , 2) * v.z));
            v2.y = ((m(1, 0) * v.x) + (m(1 , 1) * v.y) + (m(1 , 2) * v.z));
            v2.z = ((m(2, 0) * v.x) + (m(2, 1) * v.y) + (m(2 , 2) * v.z));

            return v2;
        }

        friend Matrix<T> operator*(const Matrix<T> &m1, const Matrix<T> &m2) // Times a matrix by a matrix and returns a matrix
        {
            // NOTE: code fully commented in operator*= overload for matrix multiplication
            if (m1.Columns() != m2.Rows()) return Matrix<T>(3, 3);
            Matrix<T> m3 = Matrix<T>(m1.Rows(), m2.Columns());

            for (unsigned int i = 0; (i < m3.Rows()); i++)
            {
                for (unsigned int j = 0; (j < m3.Columns()); j++)
                {
                    T sum = 0.0f;
                    for (unsigned int k = 0; (k < m1.Columns()); k++) sum += (m1(i, k) * m2(k, j));
                    m3.elements[j][i] = sum;
                }
            }

            return m3;
        }


	};










/*
 * File:   Matrix.cpp
 * Author: Donald
 *
 * Created on October 6, 2008, 3:12 PM
 * Updated to support templates on December 3, 2008, 4:37 PM
 *
 * The reason for putting the source file inside the header was that
 * it is required for template classes.
 */


    /* Constructors. */

    template<typename T>
    Matrix<T>::Matrix()
    {
        rows = columns = 3;

        // Creates an empty, 3x3 matrix
        elements = ElemArray(Columns());
        for (unsigned int i = 0; (i < Columns()); i++)
          elements[i] = Column(Rows());
    }

    template<typename T>
    Matrix<T>::Matrix(int n)
    {
        rows = columns = n;

        // Initializes vector and gives it n number of columns
        elements = ElemArray(Columns());
        // Now it initializes each column and gives n number of elements
        for (unsigned int i = 0; (i < Columns()); i++)
            elements[i] = Column(Rows());
    }

    template<typename T>
    Matrix<T>::Matrix(int nr, int nc)
    {
        rows = nr;
        columns = nc;

        // Initializes vector and gives it nc number of columns
        elements = ElemArray(Columns());
        // Now it initializes each column and gives nr number of elements
        for (unsigned int i = 0; (i < Columns()); i++)
            elements[i] = Column(Rows());
    }

    template<typename T>
    Matrix<T>::Matrix(int nr, int nc, const T* data)
    {
        rows = nr;
        columns = nc;

        // Initializes vector and gives it nc number of columns
        elements = ElemArray(Columns());
        /* Now it initializes each column and gives nr number of elements
         * It also fills the columns with the values given through data. */
        for (unsigned int i = 0; (i < Columns()); i++)
        {
            elements[i] = Column(Rows()); // Creates column with the required amount of elements

            /* Loops for the amount of elements there is newly created column
             * and then fills it in with the values given. (i + (nc * j)) is the equation used
             * to find the right value for the current part of the column vector. It works
             * perfectly assuming that the user follows the format specified in the header file. */
            for (unsigned int j = 0; (j < Rows()); j++)
                elements[i][j] = data[i + (Columns() * j)];
        }

    }

    template<typename T>
    Matrix<T>::Matrix(const Matrix<T> &m) : elements(m.elements), rows(m.Rows()), columns(m.Columns()) {}



    /* Operator overloads. */

    template<typename T>
    Matrix<T> &Matrix<T>::operator=(const Matrix<T> &m)
    {
        rows = m.Rows();
        columns = m.Columns();
        elements = m.elements;

        // Return reference to this matrix, even if new elements have not been assigned =(
        return *this;
    }

    template<typename T>
    Matrix<T> &Matrix<T>::operator*=(T m)
    {
        // Multiplies every element of the matrix with the scalar value given
        for (int i = 0; (i < Columns()); i++)
            for (int j = 0; (j < Rows()); j++)
                elements[i][j] *= m;

        return *this;
    }
    template<typename T>
    Matrix<T> &Matrix<T>::operator*=(const Matrix<T> &m)
    {
        /* First checks if rows in this matrix are equal to the columns in the given
         * matrix, if they're not, return this unaltered matrix. */
        if (Columns() != m.Rows()) return *this;
        Matrix<T> m2 = Matrix<T>(Rows(), m.Columns()); // The product of the multiplication, with the correct sizes

        /* Loops through both matrices, multiplying each row of the first matix by
         * each column of the second matrix, adding up the sums as it goes along. */
        for (int i = 0; (i < m2.Rows()); i++)
        {
            for (int j = 0; (j < m2.Columns()); j++)
            {
                // Sum of the operation
                T sum = 0.0f;
                // Multiplies elements of this row with m's column elements and adds the product
                for (int k = 0; (k < Columns()); k++) sum += (elements[k][i] * m(k, j));
                // Sum is assigned to the appropriate element
                m2.elements[j][i] = sum;
            }
        }

        // Assigns the newly created matrix's elements and size to itself and then returns
        elements = m2.elements;
        rows = m2.Rows(); columns = m2.Columns();
        return *this;
    }
    template<typename T>
    Matrix<T> &Matrix<T>::operator+=(const Matrix<T> &m)
    {
        if (SameDimensions(m))
        {
            // Adds every element of the matrix with the corresponing element in matrix m
            for (int i = 0; (i < Columns()); i++)
                for (int j = 0; (j < Rows()); j++)
                    elements[i][j] += m.elements[i][j];
        }

        return *this;
    }
    template<typename T>
    Matrix<T> &Matrix<T>::operator-=(const Matrix<T> &m)
    {
        if (SameDimensions(m))
        {
            // Subtracts every element of the matrix with the corresponing element in matrix m
            for (int i = 0; (i < Columns()); i++)
                for (int j = 0; (j < Rows()); j++)
                    elements[i][j] -= m.elements[i][j];
        }

        return *this;
    }



    template<typename T>
    Matrix<T> Matrix<T>::operator -()
    {
        // Creates a matrix the same size as this one and negates all the numbers in it
        Matrix m(3, 3);
        m.Negate();

        return m;
    }



    template<typename T>
    bool Matrix<T>::operator==(Matrix<T> &m)
    {
        if (SameDimensions(m))
	    {
	        for (int i = 0; (i < Columns()); i++)
            {
                for (int j = 0; (j < Rows()); j++)
                    // If element is not the same, the matrix isn't equal - RETURN FALSE!!!
                    if (elements[i][j] != m.elements[i][j]) return false;
            }
            /* It's passed the check and all the elements of the matrices are equal
             * so we return true to tell the program that it's the same. */
	        return true;
	    }
        // Returns false if dimensions are not equal
        else return false;
    }
    template<typename T>
    bool Matrix<T>::operator!=(Matrix<T> &m)
    {
        if (SameDimensions(m))
	    {
            for (int i = 0; (i < Columns()); i++)
            {
                for (int j = 0; (j < Rows()); j++)
                    // If element is not the same, the matrix isn't equal - YAY! RETURN TRUE!!!
                    if(elements[i][j] != m.elements[i][j]) return true;
            }
	    // It's passed the check, which means the matrices are the same...oh, well. D=
	    return false;
	    }
        /* If it's not the same dimensions, we return true, because obviously
         * the two matrices aren't the same. */
	    else return true;
    }



    /* Methods. */

    template<typename T>
    bool Matrix<T>::SameDimensions(const Matrix<T> &m)
    {
        if ((Rows() == m.Rows()) && (Columns() == m.Columns()))
            return true;
        else
            return false;
    }

    template<typename T>
    Matrix<T> Matrix<T>::ToIdentity()
    {
        for (int i = 0; (i < Columns()); i++)
        {
            // If i is greater than the amount of rows, break the loop to stop
            if (i >= Rows()) break;

            // Sets diagonals to 1
            elements[i][i] = 1;
        }
    }

    template<typename T>
    Matrix<T> Matrix<T>::Zero()
    {
        // Creates a matrix the same size as this one and fills it with zeroes
        Matrix m(Rows(), Columns());
        for (int i = 0; (i < Columns());  i++)
          for (int j = 0; (j < Rows()); j++)
            m.elements[i][j] = 0.0f;

        return m; // Returns the created zero matrix
    }

    template<typename T>
    Matrix<T> Matrix<T>::Transpose()
    {
        // Swaps number of rows and columns around for the transposed matrix
        Matrix<T> transpose(Columns(), Rows());
        // Fills the transposed matrix with the current matrix's values
        for (unsigned int i = 0; (i < transpose.Columns()); ++i)
            for (unsigned int j = 0; (j < transpose.Rows()); ++j)
                transpose.elements[i][j] = elements[j][i];
        // Returns the transposed matrix
        return transpose;
    }

    template<typename T>
    const Matrix<T>& Matrix<T>::Negate()
    {
        // Makes every element in the matrix its negative equivilent
        for (int i = 0; (i < Columns());  i++)
          for (int j = 0; (j < Rows()); j++)
            elements[i][j] = -elements[i][j];
    }

    template<typename T>
    Matrix<T> Matrix<T>::Cofactor() const
    {
        // Holds the cofactor matrix
        Matrix<T> cofactor(Rows(), Columns());
        // Temporarily holds a minor matrix
        Matrix<T> minor((Rows() - 1), (Columns() - 1));

        for (unsigned int i = 0; (i < cofactor.Rows()); ++i)
        {
            for (unsigned int j = 0; (j < cofactor.Columns()); ++j)
            {
                // Fills minor matrix with elements
                minor.elements = elements;
                // Erases the column and row containing the cofactor (column 'j', row 'i')
                minor.elements.erase(minor.elements.begin() + j);
                for (unsigned int k = 0; (k < minor.elements.size()); ++k)
                {
                    minor.elements[k].erase(minor.elements[k].begin() + i);
                }
                // Places the minor's determinant into the cofactor matrix
                if ((i + j) % 2)
                    cofactor.elements[j][i] = -minor.Determinant();
                else
                    cofactor.elements[j][i] = minor.Determinant();
            }
        }
        // Returns the calculated cofactor matrix
        return cofactor;
    }

    template<typename T>
    Matrix<T> Matrix<T>::Adjoint() const
    {
        // Returns transposed matrix of cofactors
        return Cofactor().Transpose();
    }

    template<typename T>
    T Matrix<T>::Determinant() const
    {
        /* Sources used for this algorithm:
         * http://cnx.org/content/m18289/latest/
         * NOTE: There are many oppurtunities to optimise this, so keep
         * this method in mind if the engine needs to be optimised.
         */
        T determinant = 0;
        // Just return 0 if the matrix isn't square or is lower than 2x2
        if (Rows() != Columns() || Rows() < 2 || Columns() < 2) return determinant;
        // If the matrix is 2x2, we terminate the recursion
        else if (Rows() == 2 && Columns() == 2)
        {
            // Gets the two products and subtracts one from the other
            determinant = (elements[0][0] * elements[1][1]) - (elements[1][0] * elements[0][1]);
        }
        // Otherwise, we use the expansion by minors method
        else
        {
            std::vector<T> values; // Holds values for every generated matrix
            // Calculates size of minor matrices used in the calculation
            unsigned int minorRows = (Rows() - 1),
                minorColumns = (Columns() - 1);
            /* Reserves enough memory for the highest possible amount of values that
             * will be stored in the factor in an attempt to optimise this method. */
            values.reserve(Rows() * Columns());

            // Goes through every element on the top row of the matrix
            for (unsigned int i = 0; (i < Columns()); i++)
            {
                Matrix<T> mat(minorRows, minorColumns);
                // Calculates the values of the new matrix
                mat.elements = elements;
                // Erases the column and row containing the cofactor (column 'i')
                mat.elements.erase(mat.elements.begin() + i);
                for (unsigned int j = 0; (j < mat.elements.size()); j++)
                {
                    /* Always delete the top row from each column, since the cofactors
                    * are always on the top row. */
                    mat.elements[j].erase(mat.elements[j].begin());
                }

                /* Multiplies the number from the top row with the new minor matrix and
                 * adds/subtracts it to the total determinant. +/- is done sequentially like
                 * so: +-+-+-+-+... */
                if (i % 2)
                    determinant -= (elements[i][0] * mat.Determinant());
                else
                    determinant += (elements[i][0] * mat.Determinant());
            }
        }

        // Returns the calculated determinant
        return determinant;
    }

    template<typename T>
    Matrix<T> Matrix<T>::Inverse() const
    {
        // If matrix is not square, return blank matrix since there is no inverse
        if (!IsSquare()) return Matrix<T>(Rows(), Columns());

        // Gets the determinant of the matrix
        T determinant = Determinant();
        // Gets the transposed cofactors of this matrix
        Matrix<T> adjoint = Adjoint();
        // Performs the neccessery calculation and returns the resultant matrix
        return ((1 / determinant) * adjoint);
    }
    template<typename T>
    const Matrix<T>& Matrix<T>::ToInverse()
    {
        *this = (*this).Inverse();
        return *this;
    }

    template<typename T>
    const Matrix<T>& Matrix<T>::FromYawPitchRoll(T yawDegrees, T pitchDegrees, T rollDegrees)
    {
        // Convert the given degrees into radians
        yawDegrees = DegreesToRadians(yawDegrees);
        pitchDegrees = DegreesToRadians(pitchDegrees);
        rollDegrees = DegreesToRadians(rollDegrees);
        // Calculate needed cosine/sines
        float cosY = cosf(yawDegrees);
        float cosP = cosf(pitchDegrees);
        float cosR = cosf(rollDegrees);
        float sinY = sinf(yawDegrees);
        float sinP = sinf(pitchDegrees);
        float sinR = sinf(rollDegrees);

        // Calculates the matrix's values
        Matrix<T> mat(4);
        mat.elements[0][0] = (cosR * cosY - sinR * sinP * sinY);
        mat.elements[0][1] = (sinR * cosY + cosR * sinP * sinY);
        mat.elements[0][2] = (-cosP * sinY);
        mat.elements[0][3] = 0.0f;

        mat.elements[1][0] = (-sinR * cosP);
        mat.elements[1][1] = (cosR * cosP);
        mat.elements[1][2] = sinP;
        mat.elements[1][3] = 0.0f;

        mat.elements[2][0] = (cosR * sinY + sinR * sinP * cosY);
        mat.elements[2][1] = (sinR * sinY - cosR * sinP * cosY);
        mat.elements[2][2] = (cosP * cosY);
        mat.elements[2][3] = 0.0f;

        mat.elements[3][0] = 0.0f;
        mat.elements[3][1] = 0.0f;
        mat.elements[3][2] = 0.0f;
        mat.elements[3][3] = 1.0f;

        // Assigns values to this matrix
        *this = mat;
        return *this;
    }

    template<typename T>
    const Matrix<T>& Matrix<T>::ToRotationX(T degrees)
    {
        // Clears matrix by making it a 4x4 identity matrix
        *this = Matrix<T>::Identity(4);
        // Caluclates the rotation values
        T phi = DegreesToRadians(degrees);
        T sinA = sin(phi), cosA = cos(phi);
        // Assigns the values to the appropriate elements
        elements[1][1] = cosA;
        elements[1][2] = sinA;
        elements[2][1] = -sinA;
        elements[2][2] = cosA;

        return *this;
    }

    template<typename T>
    const Matrix<T>& Matrix<T>::ToRotationY(T degrees)
    {
        // Clears matrix by making it a 4x4 identity matrix
        *this = Matrix<T>::Identity(4);
        // Caluclates the rotation values
        T theta = DegreesToRadians(degrees);
        T sinA = sin(theta), cosA = cos(theta);
        // Assigns the values to the appropriate elements
        elements[0][0] = cosA;
        elements[0][2] = -sinA;
        elements[3][0] = sinA;
        elements[3][2] = cosA;

        return *this;
    }

    template<typename T>
    const Matrix<T>& Matrix<T>::ToRotationZ(T degrees)
    {
        // Clears matrix by making it a 4x4 identity matrix
        *this = Matrix<T>::Identity(4);
        // Caluclates the rotation values
        T psi = DegreesToRadians(degrees);
        T sinA = sin(psi), cosA = cos(psi);
        // Assigns the values to the appropriate elements
        elements[0][0] = cosA;
        elements[0][1] = sinA;
        elements[1][0] = -sinA;
        elements[1][1] = cosA;

        return *this;
    }

    template<typename T>
    const Matrix<T>& Matrix<T>::ToOrthoProjection(T left, T right, T bottom, T top, T zNear, T zFar)
    {
        *this = Matrix<T>::Identity(4);

        elements[0][0] = (2 / (right - left));
        elements[1][1] = (2 / (top - bottom));
        elements[2][2] = (2 / (zFar - zNear));

        elements[0][3] = -((right + left) / (right - left));
        elements[1][3] = -((top + bottom) / (top - bottom));
        elements[2][3] = -((zFar + zNear) / (zFar - zNear));

        return *this;
    }


    /* Inline Methods. */

    template<typename T>
    inline const unsigned int &Matrix<T>::Rows() const { return rows; }

    template<typename T>
    inline const unsigned int &Matrix<T>::Columns() const { return columns; }

    template<typename T>
    inline const T &Matrix<T>::operator()(int r, int c) const
    { return elements[c][r]; }

    template<typename T>
    inline const T &Matrix<T>::Element(int r, int c) const
    { return elements[c][r]; }

    template<typename T>
    inline void Matrix<T>::SetElement(const int& r, const int& c, const T& newElem)
    { elements[c][r] = newElem; }

    template<typename T>
    inline T* Matrix<T>::Address(int r, int c)
    { return &elements[c][r]; }

    template<typename T>
    inline T* Matrix<T>::ColumnAddress(int c)
    { return &elements[c][0]; }

    template<typename T>
    inline bool Matrix<T>::IsSquare() const
    { return (Rows() == Columns()); }

    template<typename T>
    inline T* Matrix<T>::ToArray() const
    {
        // Creates an (r x c) sized array to hold the values in
        T* array = new T[Rows() * Columns()];

        // Fills the array with matrix's elements
        for (unsigned int i = 0; (i < Columns()); i++)
            for (unsigned int j = 0; (j < Rows()); j++)
                array[i + (Columns() * j)] = elements[i][j];

        // Returns created array
        return array;
    }
    template<typename T>
    inline void Matrix<T>::ToArray(T* array) const
    {
        // Fills the array given with matrix's elements
        for (unsigned int i = 0; (i < Columns()); i++)
            for (unsigned int j = 0; (j < Rows()); j++)
                array[i + (Columns() * j)] = elements[i][j];
    }


    template<typename T>
    void Matrix<T>::Print() const
    {
        // Prints out rows and columbs
        std::cout << "Matrix information:" << std::endl << std::endl
            << "Rows: " << Rows() << " Columns: " << Columns() << std::endl;

        // Prints out the matrix's elements
        for (unsigned int c = 0; (c < Columns()); c++)
        {
            for (unsigned int r = 0; (r < Rows()); r++)
            {
                std::cout << general::ToString<T>(r) << general::ToString<T>(c) <<
                    ": " << elements[c][r] << std::endl;
            }
            std::cout << std::endl;
        }
    }

    template<typename T>
    Vector3<T> Matrix<T>::GetColumnAsVector3(const int& c)
    {
        // If the matrix deosn't have 3 or 4 rows and columns, return a blank matrix
        if ((Columns() < 3) || (Columns() > 4) || (Rows() < 3) || (Rows() > 4))
            return Vector3<T>();

        // Creates vector and fills it with the requested values
        Vector3<T> vec;
        vec.x = elements[c][0];
        vec.y = elements[c][1];
        vec.z = elements[c][2];

        return vec; // Returns the created vector
    }


    // A few typedefs to make life easier
    typedef Matrix<int> matrixi;
    typedef Matrix<float> matrixf;
    typedef Matrix<double> matrixd;



}

}

#endif	/* _MATRIX_H */
