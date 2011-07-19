/*
 * File:   Program.cpp
 * Author: Donald "Datriot" Whyte
 *
 * Created on April 11, 2009, 10:25 AM
 */

#include "Program.h"
#include "Exceptions.h"

namespace parcel
{

namespace graphics
{

    Program::Program() : linked(false), enabled(false)
    {
        glHandle = glCreateProgram();
    }

    Program::~Program()
    {
        /* Makes sure to delete every shader held in this program by
         * explicitly calling their Delete() method. */
        for (std::map<std::string, Shader*>::iterator it = shaders.begin();
            (it != shaders.end()); it++)
        {
            delete it->second;
        }

        // Then deletes the program
        glDeleteProgram(glHandle);
    }


    bool Program::Link()
    {
        // Makes sure the program isn't already linked
        if (linked) return true;

        glLinkProgram(glHandle);

        // Checks if the program linked successfully
        glGetProgramiv(glHandle, GL_LINK_STATUS, (GLint*)&linked);
        // Just return true if it linked successfully
        if (linked)
        {
            return true;
        }
        // If it never linked successfully, prints error message and returns false
        else
        {
            // Finds out what went wrong by reading the info log
            GLint infologLength = 0; // Used to get how long the log is
            GLsizei charsWritten = 0; // Stores how many characters that are written to infolog
            std::string infolog = ""; // Stores info log characters

            // Gets length of the infolog
            glGetProgramiv(glHandle, GL_INFO_LOG_LENGTH, &infologLength);

            if (infologLength > 0)
            {
                // Resizes string to correct size and then stores infolog in it
                infolog.resize(infologLength);
                glGetProgramInfoLog(glHandle, infologLength, &charsWritten, &infolog[0]);
            }

            // Prints the error message
            std::cout << "Program::Link - Program failed to link!" << std::endl
                << infolog << std::endl;

            return false;
        }
    }


    void Program::Enable()
    {
        // Make sure that the program is linked before using it
        if (!linked) return;

        glUseProgram(glHandle);

        enabled = true;
    }

    void Program::Disable()
    {
        // Just returns if the program hasn't been linked yet
        if (!linked) return;

        // Disables the current program, returning to fixed functionality
        glUseProgram(0);

        enabled = false;
    }


    bool Program::AddShader(Shader* shader, const std::string& id, bool attach)
    {
        // Checks if given ID is already taken
        std::map<std::string, Shader*>::iterator it = shaders.find(id);

        // If it couldn't find a shader with the given ID...
        if (it == shaders.end())
        {
            // ...add the shader to the map
            shaders[id] = shader;
            // Also sets the shader's program handle to this one
            std::map<std::string, Shader*>::iterator it = shaders.find(id);
            it->second->SetProgramHandle(glHandle);

            // Attaches the shader to this program if specified
            if (attach)
            {
                it->second->Attach();
            }

            return true;
        }
        /* Otherwise, it found a shader with the given ID so... don't add shader to
         * the map, return false =( */
        else
        {
            delete shader;

            return false;
        }
    }

    bool Program::RemoveShader(const std::string& id)
    {
        // Tries to find a shader with the given id
        std::map<std::string, Shader*>::iterator it = shaders.find(id);

        // If it found the shader delete it
        if (it != shaders.end())
        {
            // Automatically detaches the shader from the program
            it->second->Detach();
            // Deletes shader and erases pointer from map
            delete it->second;
            shaders.erase(it);

            return false;
        }
        // Otherwise, just return false
        else
        {
            return false;
        }
    }


    Shader* Program::GetShader(const std::string& id)
    {
        // Tries to find and return the shader with the given ID
        std::map<std::string, Shader*>::iterator it = shaders.find(id);

        if (it != shaders.end())
        {
            return it->second;
        }
        else
        {
            return NULL;
        }
    }


    std::vector<float> Program::GetUniformFloat(const std::string& varName, unsigned int amount) const
    {
        // Gets the location of the specified variable
        GLint location = GetVariableLocation(varName, true);
        // Holds all the values retrieved
        std::vector<float> values;
        // Resizes vector to the amount of values to read
        values.resize(amount);
        // Retrives the values from the variable and returns them
        glGetUniformfv(glHandle, location, &values[0]);

        return values;
    }
    maths::matrixf Program::GetUniformMatrixFloat(const std::string& varName, unsigned int rows,
        unsigned int columns) const
    {
        // First of all, makes sure that the given sizes for the matrix are compatible
        if (rows < 2 || rows > 4 || columns < 2 || columns > 4)
        {
            throw debug::InvalidArgumentException(
                "Program::GetUniformMatrix - Rows or columns is an invalid size, cannot get the values.");
        }

        // Gets the location of the uniform matrix
        GLint location = GetVariableLocation(varName, true);
        // Declares and initialises an array to hold the retrieved values
        float* values = new float[rows * columns];
        // Gets the values and stores the in the above array
        glGetUniformfv(glHandle, location, &values[0]);

        // Creates a matrix filled with the retrieved values
        maths::matrixf mat(rows, columns, values);
        // Makes sure to delete the values
        delete values;
        // Returns the matrix
        return mat;
    }

    void Program::SetUniform(const std::string& varName, const std::vector<float>& values)
    {
        // Gets location of variable and the amount of values to set
        GLint location = GetVariableLocation(varName, true);
        unsigned int amount = values.size();

        if (amount == 0)
        {
            throw debug::InvalidArgumentException(
                "Program::SetUniform - Cannot pass zero amount of values!");
        }

        // Sets the values
        glUniform1fv(location, amount, &values[0]);
    }
    void Program::SetUniform(const std::string& varName, const std::vector<maths::vector2f>& values)
    {
        GLint location = GetVariableLocation(varName, true);
        unsigned int amount = values.size();
        if (amount == 0) throw debug::InvalidArgumentException("Program::SetUniform - Cannot pass zero amount of values!");
        glUniform2fv(location, amount, &values[0].x);
    }
    void Program::SetUniform(const std::string& varName, const std::vector<maths::vector3f>& values)
    {
        GLint location = GetVariableLocation(varName, true);
        unsigned int amount = values.size();
        if (amount == 0) throw debug::InvalidArgumentException("Program::SetUniform - Cannot pass zero amount of values!");
        glUniform3fv(location, amount, &values[0].x);
    }
    void Program::SetUniform(const std::string& varName, const std::vector<maths::vector4f>& values)
    {
        GLint location = GetVariableLocation(varName, true);
        unsigned int amount = values.size();
        if (amount == 0) throw debug::InvalidArgumentException("Program::SetUniform - Cannot pass zero amount of values!");
        glUniform4fv(location, amount, &values[0].x);
    }
    void Program::SetUniformMatrix(const std::string& varName, const maths::matrixf& mat)
    {
        // Finds location of variable
        GLint location = GetVariableLocation(varName, true);
        // Gets size of the matrix
        const int& rows = mat.Rows(), columns = mat.Columns();
        // Puts matrix's values into an array
        float* matValues = mat.ToArray();

        // Checks the size of the given matrix to call the appropriate function
        if (rows == 2 && columns == 2) glUniformMatrix2fv(location, 1, false, matValues);
        else if (rows == 2 && columns == 3) glUniformMatrix2x3fv(location, 1, false, matValues);
        else if (rows == 2 && columns == 4) glUniformMatrix2x4fv(location, 1, false, matValues);
        else if (rows == 3 && columns == 2) glUniformMatrix3x2fv(location, 1, false, matValues);
        else if (rows == 3 && columns == 3) glUniformMatrix3fv(location, 1, false, matValues);
        else if (rows == 3 && columns == 4) glUniformMatrix3x4fv(location, 1, false, matValues);
        else if (rows == 4 && columns == 2) glUniformMatrix4x2fv(location, 1, false, matValues);
        else if (rows == 4 && columns == 3) glUniformMatrix4x3fv(location, 1, false, matValues);
        else if (rows == 4 && columns == 4) glUniformMatrix4fv(location, 1, false, matValues);
        // Otherwise, the size is not compatible; throw an exception
        else
        {
            delete matValues;
            throw debug::InvalidArgumentException("Program::SetUniformMatrix - A matrix with an invalid size has been passed.");
        }

        delete matValues; // Makes sure the allocated memory is released
    }


    std::vector<int> Program::GetUniformInt(const std::string& varName, unsigned int amount) const
    {
        GLint location = GetVariableLocation(varName, true);
        std::vector<int> values;
        values.resize(amount);
        glGetUniformiv(glHandle, location, &values[0]);
        return values;
    }
    maths::matrixi Program::GetUniformMatrixInt(const std::string& varName, unsigned int rows,
        unsigned int columns) const
    {
        // First of all, makes sure that the given sizes for the matrix are compatible
        if (rows < 2 || rows > 4 || columns < 2 || columns > 4)
        {
            throw debug::InvalidArgumentException(
                "Program::GetUniformMatrix - Rows or columns is an invalid size, cannot get the values.");
        }

        // Gets the location of the uniform matrix
        GLint location = GetVariableLocation(varName, true);
        // Declares and initialises an array to hold the retrieved values
        int* values = new int[rows * columns];
        // Gets the values and stores the in the above array
        glGetUniformfv(glHandle, location, (GLfloat*)&values[0]);

        // Creates a matrix filled with the retrieved values
        maths::matrixi mat(rows, columns, values);
        // Makes sure to delete the values
        delete values;
        // Returns the matrix
        return mat;
    }

    void Program::SetUniform(const std::string& varName, const std::vector<int>& values)
    {
        GLint location = GetVariableLocation(varName, true);
        unsigned int amount = values.size();
        if (amount == 0) throw debug::InvalidArgumentException("Program::SetUniform - Cannot pass zero amount of values!");
        glUniform1iv(location, amount, &values[0]);
    }
    void Program::SetUniform(const std::string& varName, const std::vector<maths::vector2i>& values)
    {
        GLint location = GetVariableLocation(varName, true);
        unsigned int amount = values.size();
        if (amount == 0) throw debug::InvalidArgumentException("Program::SetUniform - Cannot pass zero amount of values!");
        glUniform2iv(location, amount, &values[0].x);
    }
    void Program::SetUniform(const std::string& varName, const std::vector<maths::vector3i>& values)
    {
        GLint location = GetVariableLocation(varName, true);
        unsigned int amount = values.size();
        if (amount == 0) throw debug::InvalidArgumentException("Program::SetUniform - Cannot pass zero amount of values!");
        glUniform3iv(location, amount, &values[0].x);
    }
    void Program::SetUniform(const std::string& varName, const std::vector<maths::vector4i>& values)
    {
        GLint location = GetVariableLocation(varName, true);
        unsigned int amount = values.size();
        if (amount == 0) throw debug::InvalidArgumentException("Program::SetUniform - Cannot pass zero amount of values!");
        glUniform4iv(location, amount, &values[0].x);
    }
    void Program::SetUniformMatrix(const std::string& varName, const maths::matrixi& mat)
    {
        GLint location = GetVariableLocation(varName, true);
        const int& rows = mat.Rows(), columns = mat.Columns();
        int* matValues = mat.ToArray();

        /* NOTE: No integer equivilents for matrices, so just pass the integers
         * and make them act as floats. */
        if (rows == 2 && columns == 2) glUniformMatrix2fv(location, 1, false, (GLfloat*)matValues);
        else if (rows == 2 && columns == 3) glUniformMatrix2x3fv(location, 1, false, (GLfloat*)matValues);
        else if (rows == 2 && columns == 4) glUniformMatrix2x4fv(location, 1, false, (GLfloat*)matValues);
        else if (rows == 3 && columns == 2) glUniformMatrix3x2fv(location, 1, false, (GLfloat*)matValues);
        else if (rows == 3 && columns == 3) glUniformMatrix3fv(location, 1, false, (GLfloat*)matValues);
        else if (rows == 3 && columns == 4) glUniformMatrix3x4fv(location, 1, false, (GLfloat*)matValues);
        else if (rows == 4 && columns == 2) glUniformMatrix4x2fv(location, 1, false, (GLfloat*)matValues);
        else if (rows == 4 && columns == 3) glUniformMatrix4x3fv(location, 1, false, (GLfloat*)matValues);
        else if (rows == 4 && columns == 4) glUniformMatrix4fv(location, 1, false, (GLfloat*)matValues);
        else
        {
            delete matValues;
            throw debug::InvalidArgumentException("Program::SetUniformMatrix - A matrix with an invalid size has been passed.");
        }

        delete matValues;
    }


    void Program::EnableAttributeArray(const std::string& varName)
    {
        // Gets location of the vertex attribute
        GLint location = GetVariableLocation(varName, false);
        // Enables a vertex attribute array that will fill the attribute specified every vertex
        glEnableVertexAttribArray(location);
    }
    void Program::DisableAttributeArray(const std::string& varName)
    {
        // Same as above, but disables the array instead
        GLint location = GetVariableLocation(varName, false);
        glDisableVertexAttribArray(location);
    }

    void Program::FillAttributeArray(const std::string& varName, unsigned char memorySpacing,
        const std::vector<float>& values)
    {
        GLint location = GetVariableLocation(varName, false);
        /* Fills the attribute array with the given values. Because floats are being passed,
         * we use 1 as the amount of values for an individual vertex. */
        glVertexAttribPointer(location, 1, GL_FLOAT, 0, memorySpacing, &values[0]);
    }
    void Program::FillAttributeArray(const std::string& varName, unsigned char memorySpacing,
        const std::vector<maths::vector2f>& values)
    {
        GLint location = GetVariableLocation(varName, false);
        // 2 because vector2f contains two values
        glVertexAttribPointer(location, 2, GL_FLOAT, 0, memorySpacing, &values[0].x);
    }
    void Program::FillAttributeArray(const std::string& varName, unsigned char memorySpacing,
        const std::vector<maths::vector3f>& values)
    {
        GLint location = GetVariableLocation(varName, false);
        glVertexAttribPointer(location, 3, GL_FLOAT, 0, memorySpacing, &values[0].x);
    }
    void Program::FillAttributeArray(const std::string& varName, unsigned char memorySpacing,
        const std::vector<maths::vector4f>& values)
    {
        GLint location = GetVariableLocation(varName, false);
        glVertexAttribPointer(location, 4, GL_FLOAT, 0, memorySpacing, &values[0].x);
    }

    void Program::FillAttributeArray(const std::string& varName, unsigned char memorySpacing,
        const std::vector<int>& values)
    {
        GLint location = GetVariableLocation(varName, false);
        glVertexAttribPointer(location, 1, GL_INT, 0, memorySpacing, &values[0]);
    }
    void Program::FillAttributeArray(const std::string& varName, unsigned char memorySpacing,
        const std::vector<maths::vector2i>& values)
    {
        GLint location = GetVariableLocation(varName, false);
        glVertexAttribPointer(location, 2, GL_INT, 0, memorySpacing, &values[0].x);
    }
    void Program::FillAttributeArray(const std::string& varName, unsigned char memorySpacing,
        const std::vector<maths::vector3i>& values)
    {
        GLint location = GetVariableLocation(varName, false);
        glVertexAttribPointer(location, 3, GL_INT, 0, memorySpacing, &values[0].x);
    }
    void Program::FillAttributeArray(const std::string& varName, unsigned char memorySpacing,
        const std::vector<maths::vector4i>& values)
    {
        GLint location = GetVariableLocation(varName, false);
        glVertexAttribPointer(location, 4, GL_INT, 0, memorySpacing, &values[0].x);
    }


    GLint Program::GetVariableLocation(const std::string& varName, bool isUniform) const
    {
        // Depending on isUniform, search for a uniform or attribute variable
        GLint location;
        if (isUniform)
        {
            location = glGetUniformLocation(glHandle, varName.c_str());
        }
        else
        {
            location = glGetAttribLocation(glHandle, varName.c_str());
        }

        // If the returned location is -1, OpenGL could not find the specified variable
        if (location == -1)
        {
            throw debug::InvalidArgumentException(
                "Program::GetVariableLocation - Specified variable '"
                + varName + "' could not be found.");
        }
        // Otherwise, it succeded, return the location
        else
        {
            return location;
        }
    }

}

}
