/*
 * File:   Shader.h
 * Author: Donald "Datriot" Whyte
 *
 * Created on April 10, 2009, 7:04 PM
 */

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GLee.h>

namespace parcel
{

namespace graphics
{

    /* Contains every shader type. */
    enum ShaderType
    {
        SHADERTYPE_VERTEX,
        SHADERTYPE_FRAGMENT,
        SHADERTYPE_GEOMETRY
    };

    /* This calls creates and cleans up a shader. It loads the shader source
     * file in the constructor and passes it to the OpenGL shader. */
    class Shader
    {


    private:

        std::string source; // Source code of the shader
        ShaderType type; // Type of shader

        bool compiled; // Shader compile status
        bool attachedToProgram; // Is it attached to a program or not?

        GLint glHandle; // OpenGL ID for this shader
        GLint programHandle; // OpenGL ID for its program


        /* Loads a shaer file's source code and returns it. */
        std::string LoadShaderFile(const std::string& filename);


    public:

        /* The constructor creates the shader object then loads the source
         * code from the given file into the shader object. If the file is not
         * found, throw an exception. The destructor simply deletes the
         * shader object. */
        Shader(ShaderType shaderType, const std::string& filename);
        ~Shader();

        /* Compiles the shader. If compilation failed, then print the error
         * and infolog. */
        bool Compile();

        /* Attaches and detaches the shader from the program its holding
         * respectively. */
        void Attach();
        void Detach();

        /* Getters/setters. */
        const bool& IsCompiled() { return compiled; }
        const bool& IsAttached() { return attachedToProgram; }
        void SetProgramHandle(GLint progHandle) { programHandle = progHandle; }



    };

}

}

#endif
