/*
 * File:   Program.h
 * Author: Donald "Datriot" Whyte
 *
 * Created on April 10, 2009, 7:12 PM
 */

#ifndef PROGRAM_H
#define PROGRAM_H

#include <vector>
#include <map>
#include <string>
#include "Shader.h"
#include "Matrix.h"

namespace parcel
{

namespace graphics
{

    /* Encapsulates shader program creation, management and deletion. */
    class Program
    {


    private:

        std::map<std::string, Shader*> shaders; // Holds every shader with an ID as a lookup key

        bool linked; // True if the program was successfully linked
        bool enabled; // True if the program is currently being used by the pipeline

        GLint glHandle; // OpenGL handle to the program


        /* Returns the location of a uniform/attribute variable inside this program.
         * When isUniform is true, it looks for a uniform variable with the given
         * name. Similarly. when isUniform is false, it looks for an attribute variable
         * with the given name. Throws an exception if it fails to find the variable. */
        GLint GetVariableLocation(const std::string& varName, bool isUniform) const;


    public:

        /* Constructor and destructor. Creates and deletes the program respectively. */
        Program();
        ~Program();

        /* Links the program. Returns true when it's a success When it fails to link,
         * it prints out an info log to explain why it failed. */
        bool Link();

        /* Enables and disables the program respectively. Does nothing if the program
         * hasn't been linked beforehand. */
        void Enable();
        void Disable();

        /* Adds a shader to the map. If 'attach' is true, then the method automatically
         * attaches the shader to the program, providing that the program is not linked.
         * If the ID given has already been taken, then it returns false. If the ID has not
         * been taken, it adds it to the map and returns true.
         * NOTE: If ID is already used, it will not add the given shader AND it will
         * delete the shader object given, invalidating the pointer passed through the
         * parameters. Make sure to pass new objects through this method by using
         * 'new Shader(...)' instead of passing a pointer, sicne it may become invalidated. */
        bool AddShader(Shader* shader, const std::string& id, bool attach);
        /* Removes the shader with the given shader, before getting rid of it, it
         * makes sure the shader is detached from the program. Returns true
         * if it successfully found and deleted the shader. */
        bool RemoveShader(const std::string& id);

        /* Getters */
        Shader* GetShader(const std::string& id); // returns NULL if it couldn't find the shader
        const bool& IsEnabled() { return enabled; }

        /* The following methods are for retrieving or altering the values of the uniform
         * and attribute variables inside the shader program held in this class.
         * This is key to communicating between the host OpenGL application (this)
         * and the shaders (in this class). There are getter and setter methods for both
         * integers, floats, vectors and matrices. Simply use the integer methods when
         * getting/setting boolean variables.
         *
         * NOTE1: GetUniformFloat/Int/ ONLY return the first element in an array if the full
         * array was specified. Basically, passing myArray will be the same as passing
         * myArray[0], so only the first element is returned.
         *
         * NOTE2: Matrices passed to SetUniformMatrixFloat/Int must be a size from 2x2
         * to 4x4, otherwise an exception is thrown.
         *
         * NOTE3: Make sure the amount of rows and columns given as arguments in
         * GetUniformFloat/Int are the VALID size of the matrix in the shader. The wrong
         * size may cause undefined behaviour. Additionally, if rows or columns given is
         * higher than 4 or lower than 2, an exception is thown.
         *
         * NOTE4: Also, unlike SetUniform, SetUniformMatrix cannot only assign one
         * matrix at a time, which means if there is an array of matrices in the shader,
         * each element has to be assigned with a new call to SetUniformMatrix.
         *
         * NOTE5: Because there are arrays that are dynamically allocated in the
         * Get/SetUniformMatrixInt/Float methods, they may run slower than usual.
         * I may come up with a way of not using this in the future.
         *
         * NOTE6: There is no integer equivilent for the Get/SetUniformMatrix methods
         * because GLSL only supports matrices with contain floats. So the integers
         * passed to SetUniformMatrix are converted to floats and the floats gotten
         * from GetUniformMatrix are converted to integers. */

        /* Getting float uniforms. */
        std::vector<float> GetUniformFloat(const std::string& varName, unsigned int amount) const;
        maths::matrixf GetUniformMatrixFloat(const std::string& varName, unsigned int rows,
            unsigned int columns) const;
        /* Setting float uniforms. */
        void SetUniform(const std::string& varName, const std::vector<float>& values);
        void SetUniform(const std::string& varName, const std::vector<maths::vector2f>& values);
        void SetUniform(const std::string& varName, const std::vector<maths::vector3f>& values);
        void SetUniform(const std::string& varName, const std::vector<maths::vector4f>& values);
        void SetUniformMatrix(const std::string& varName, const maths::matrixf& mat);

        /* Getting integer uniforms. */
        std::vector<int> GetUniformInt(const std::string& varName, unsigned int amount) const;
        maths::matrixi GetUniformMatrixInt(const std::string& varName, unsigned int rows,
            unsigned int columns) const;
        /* Setting integer uniforms. */
        void SetUniform(const std::string& varName, const std::vector<int>& values);
        void SetUniform(const std::string& varName, const std::vector<maths::vector2i>& values);
        void SetUniform(const std::string& varName, const std::vector<maths::vector3i>& values);
        void SetUniform(const std::string& varName, const std::vector<maths::vector4i>& values);
        void SetUniformMatrix(const std::string& varName, const maths::matrixi& mat);

        /* Enables and disables an attribute array (much like EnableClientState()) respectively. */
        void EnableAttributeArray(const std::string& varName);
        void DisableAttributeArray(const std::string& varName);

        /* Fills in an attribute array using the given properties. Enabling/disabling the
         * array is the job of the two methods above; this method just fills an attribute
         * array's values via a call to glVertexAttribPointer(). */
        // TODO: Test the vertex attribute functions!!!

        /* Float attribute arrays. */
        void FillAttributeArray(const std::string& varName, unsigned char memorySpacing,
            const std::vector<float>& elements);
        void FillAttributeArray(const std::string& varName, unsigned char memorySpacing,
            const std::vector<maths::vector2f>& elements);
        void FillAttributeArray(const std::string& varName, unsigned char memorySpacing,
            const std::vector<maths::vector3f>& elements);
        void FillAttributeArray(const std::string& varName, unsigned char memorySpacing,
            const std::vector<maths::vector4f>& elements);

        /* Integer attribute arrays. */
        void FillAttributeArray(const std::string& varName, unsigned char memorySpacing,
            const std::vector<int>& elements);
        void FillAttributeArray(const std::string& varName, unsigned char memorySpacing,
            const std::vector<maths::vector2i>& elements);
        void FillAttributeArray(const std::string& varName, unsigned char memorySpacing,
            const std::vector<maths::vector3i>& elements);
        void FillAttributeArray(const std::string& varName, unsigned char memorySpacing,
            const std::vector<maths::vector4i>& elements);


    };

}

}

#endif
