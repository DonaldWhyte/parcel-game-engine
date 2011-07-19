/*
 * File:   LuaManager.h
 * Author: Donald "Datriot" Whyte
 *
 * Created on October 11, 2008, 7:08 PM
 * Imported into Parcel engine and modified on
 * June 3, 11:13 AM
 */

#ifndef LUAMANAGER_H
#define LUAMANAGER_H

#include <string>
#include <lua.hpp>
#include "Exceptions.h"

namespace parcel
{

namespace lua
{

    /* Exception thrown whenever something goes wrong on the lua side. */
    class LuaException : public debug::Exception
    {
    public:
        LuaException(const std::string& errorMessage) : Exception(errorMessage) {}

    };

    /* Loads and executes Lua scripts and stores the state of the virtual machine.
     * This class also bridges the gap between C++ and Lua, allowing C++ to get/set
     * variables from Lua and register C++ functions to be used in Lua. */
    class LuaManager
    {


    private:

        lua_State* vm;  // The Lua virtual machine


        /* This method pushes the global variable "varName" to the top of the stack.
         * Throws an exception if the global doesn't exitst. */
        void GetGlobal(const std::string& varName);
        /* Gets a field. This method assumes that a table has already been pushed
         * to the top of the stack. Throws an exception if the table/field doesn't exist. */
        void GetField(const std::string& field);
        /* Instead of assuming there's a table at the top of the stack lke the above
         * method, this one pushes the tabnle requested onto the stack for us. */
        void GetField(const std::string& varName, const std::string& field);
        /* Sets the value of a global variable and a field of a table respectively. It
         * does this by setting the variable with the value that at's the top of the
         * stack, so you have to push values before calling these. (you can do this by
         * calling functions like lua_ pushnumber) */
        void SetGlobal(const std::string& varName);
        void SetField(const std::string& field);


    public:

        /* Constructor, starts virtual machine and opens default libraries for use in Lua. */
        LuaManager();
        /* Destructor, closes and destroys the lua state. */
        ~LuaManager();

        /* Loads and executes a script. */
        void LoadAndExecuteScript(const std::string& filename);

        /* Registers a C function inside the Lua interpreter. */
        void RegisterFunction(luaL_reg function);
        /* Registers a collection of functions in a table called libraryName. Lua
         * supports dynamic loader libraries, but this would cause Lua to be
         * unsafe, as it can breach the confines of the engine then. */
        void RegisterStaticLibrary(const std::string& libraryName, const luaL_reg* functions);

        /* These methods attempt to acquire a value of a certain type from a variable
         * called 'varName'. If the variable in Lua is nil or isn't the type the methods
         * is looking for, they throw an exception. */
        int GetIntFromVariable(const std::string& varName);
        float GetFloatFromVariable(const std::string& varName);
        double GetDoubleFromVariable(const std::string& varName);
        bool GetBoolFromVariable(const std::string& varName);
        std::string GetStringFromVariable(const std::string& varName);
        /* Methods same as above but they get a specific field in a table. */
        int GetIntFromTable(const std::string& varName, const std::string& field);
        float GetFloatFromTable(const std::string& varName, const std::string& field);
        double GetDoubleFromTable(const std::string& varName, const std::string& field);
        bool GetBoolFromTable(const std::string& varName, const std::string& field);
        std::string GetStringFromTable(const std::string& varName, const std::string& field);

        /* Same as above, but sets/creates a variable instead of getting its value. */
        void SetIntToVariable(const std::string& varName, int value);
        void SetFloatToVariable(const std::string& varName, float value);
        void SetDoubleToVariable(const std::string& varName, double value);
        void SetBoolToVariable(const std::string& varName, bool value);
        void SetStringToVariable(const std::string& varName, const std::string& value);
        /* Table field setters. */
        void SetIntToTable(const std::string& varName, const std::string& field,
            int value);
        void SetFloatToTable(const std::string& varName, const std::string& field,
            float value);
        void SetDoubleToTable(const std::string& varName, const std::string& field,
            double value);
        void SetBoolToTable(const std::string& varName, const std::string& field,
            bool value);
        void SetStringToTable(const std::string& varName, const std::string& field,
            const std::string& value);

        /* TODO. */
//        /* Getters/setters for USERDATA type variables in Lua. Have to be implemented
//         * here, in the header, since they receive template parameters. */
//        template<typename T> T GetUserDataFromVariable(const std::string& varName)
//        {
//            // Pushes required variable to the top of the stack
//            GetGlobal(varName);
//            /* If the variable IS userdata, convert it to the required C++ datatype
//             * and return it. If it ISN'T, throw an exception. */
//            if (lua_isuserdata(vm, -1))
//            {
//                return *(reinterpret_cast<T*>(lua_touserdata(vm, -1)));
//            }
//            else
//            {
//                throw LuaException("LuaManager::GetUserDataFromVariable - '" +
//                    varName + "' is not user data!");
//            }
//        }
//
//        template<typename T> T GetUserDataFromTable(const std::string& varName,
//            const std::string& field)
//        {
//            GetField(varName, field); // Pushes requested field on to the top of the stack
//            if (lua_isuserdata(vm, -1))
//            {
//                return static_cast<T>(lua_touserdata(vm, -1));
//            }
//            else
//            {
//                throw LuaException("LuaManager::GetUserDataFromVariable - '" +
//                    varName + "' is not user data!");
//            }
//        }
//
//        template<typename T> T* SetUserDataToVariable(const std::string& varName, T* value)
//        {
//            // Allocates enough memory for the full user data
//            void* userdata = lua_newuserdata(vm, sizeof(T));
//            // Assigns the new user data the given values
//            T* convert = (T*)userdata;
//            convert = value;
//            // Passes suerdata to the required variable
//            SetGlobal(varName);
//        }
//        template<typename T> void SetUserDataToTable(const std::string& varName,
//            const std::string& field, T value)
//        {
////            GetGlobal(varName);
////            lua_pushuserdata(vm, reinterpret_cast<void*>(T));
////            SetField(field);
//        }


    };

}

}

#endif
