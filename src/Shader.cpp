/*
 * File:   Shader.cpp
 * Author: Donald "Datriot" Whyte
 *
 * Created on April 10, 2009, 9:51 PM
 */

#include <fstream>
#include "Shader.h"
#include "Exceptions.h"

namespace parcel
{

namespace graphics
{

    Shader::Shader(ShaderType shaderType, const std::string& filename)
     : type(shaderType), compiled(false), attachedToProgram(false),
     glHandle(0), programHandle(0)
    {
        try
        {
            // Gets appropriate enumerator for creating OpenGL shader...
            GLenum t = 0;
            // ...by checking this shader's ShaderType property and assigning the correct enum
            switch (type)
            {
                case SHADERTYPE_VERTEX:
                {
                    t = GL_VERTEX_SHADER;

                    break;
                }
                case SHADERTYPE_FRAGMENT:
                {
                    t = GL_FRAGMENT_SHADER;

                    break;
                }
                case SHADERTYPE_GEOMETRY:
                {
                    // If geometry shaders are not supported by the hardware, throw an exception
                    if (!GLEE_ARB_geometry_shader4)
                    {
                        throw debug::UnsupportedOperationException(
                            "Shader::Shader - Geometry shaders are not supported on this machine!");
                    }

                    t = GL_GEOMETRY_SHADER_ARB; // TODO

                    break;
                }
            }

            // Create OpenGL shader object
            glHandle = glCreateShader(t);
            // Gets the source code of the shader from given file
            source = LoadShaderFile(filename);
            // Places the source code into the newly created shader object
            const char* s = &source[0];
            glShaderSource(glHandle, 1, &s, NULL);
        }
        catch (debug::Exception& ex)
        {
            ex.PrintMessage();
        }
        catch (std::exception& ex)
        {
            std::cout << ex.what() << std::endl;
        }
        catch (...)
        {
            std::cout << "Shader::Shader - Failed for unknown reason.";
        }
    }

    Shader::~Shader()
    {
        // Makes sure to cleanup the shader used by this object
        glDeleteShader(glHandle);
    }


    bool Shader::Compile()
    {
        // Compiles the shader
        glCompileShader(glHandle);

        // Checks if the shader compiled successfully
        glGetShaderiv(glHandle, GL_COMPILE_STATUS, (GLint*)&compiled);
        // Just return true if it compiled successfully
        if (compiled)
        {
            return true;
        }
        // If it never compiled successfully, prints error message and returns false
        else
        {
            // Finds out what went wrong by reading the info log
            GLint infologLength = 0; // Used to get how long the log is
            GLsizei charsWritten = 0; // Stores how many characters that are written to infolog
            std::string infolog = ""; // Stores info log characters

            // Gets length of the infolog
            glGetShaderiv(glHandle, GL_INFO_LOG_LENGTH, &infologLength);

            if (infologLength > 0)
            {
                // Resizes string to correct size and then stores infolog in it
                infolog.resize(infologLength);
                glGetShaderInfoLog(glHandle, infologLength, &charsWritten, &infolog[0]);
            }

            // Prints the error message
            std::cout << "Shader failed to compile!" << std::endl
                << infolog << std::endl;

            return false;
        }
    }


    void Shader::Attach()
    {
        /* If the programHandle is 0 (no program) or the shader hasn't
         * been compiled yet, just return. */
        if (programHandle == 0 || !compiled) return;

        // Attaches the shader to the program
        glAttachShader(programHandle, glHandle);

        attachedToProgram = true;
    }

    void Shader::Detach()
    {
        if (programHandle == 0 || !compiled) return;

        glDetachShader(programHandle, glHandle);

        attachedToProgram = false;
    }


    std::string Shader::LoadShaderFile(const std::string& filename)
    {
        // Loads the shader file
        std::ifstream file(filename.c_str());
        if (!file.is_open())
        {
            throw debug::Exception("Shader::Shader - Could not load specified shader file.");
        }
        /* Dumps all the data from the file into the source string.
         * The first parameter is an iterator to the current character (first) in the file.
         * The second uses the iterator's default constructor, which places the iterator
         * at the end of the file. Thus it copies all the characters from the beginning to
         * end of the file into the string. */
        std::string contents = std::string(
            (std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        return contents;
    }

}

}
