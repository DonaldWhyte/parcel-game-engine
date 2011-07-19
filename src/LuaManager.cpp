/*
 * File:   LuaManager.cpp
 * Author: Donald "Datriot" Whyte
 *
 * Created on October 11, 2008, 7:08 PM
 * Imported into Parcel engine and modified on
 * June 3, 11:13 AM
 */

#include "LuaManager.h"

namespace parcel
{

namespace lua
{

    LuaManager::LuaManager()
    {
        // Starts the virtual machine
        vm = lua_open();
        // Opens default libraries
        luaL_openlibs(vm);
    }

    LuaManager::~LuaManager()
    {
        lua_close(vm);
    }

    void LuaManager::LoadAndExecuteScript(const std::string& filename)
    {
        int error; // Error checker
        /* Attempts to execute the required chunk in protected mode. */
        error = luaL_dofile(vm, filename.c_str());

        // If there was an error
        if (error)
        {
            // Gets error message and pops it off the top of the stack
            std::string message = luaL_checkstring(vm, -1);
            lua_pop(vm, -1);

            throw LuaException("LuaManager::LoadAndExecuteScript - " + message);
        }
    }

    void LuaManager::RegisterFunction(luaL_reg function)
    {
        // Registers given function in the vm
        lua_register(vm, function.name, function.func);
    }

    void LuaManager::RegisterStaticLibrary(const std::string& libraryName, const luaL_reg* functions)
    {
        // Creates a new globals table
        lua_setglobal(vm, libraryName.c_str());
        // Registers functions within that table
        luaL_register(vm, libraryName.c_str(), functions);
    }

    void LuaManager::GetGlobal(const std::string& varName)
    {
        // Pushes variable 'varName' to the top of the stack
        lua_getglobal(vm, varName.c_str());
        // If that value is nil, return false
        if (lua_isnil(vm, -1))
        {
            throw LuaException("LuaManager::GetGlobal - Cannot find global variable '" + varName + "'.");
        }
    }

    void LuaManager::GetField(const std::string& field)
    {
        // If the variable on top of the stack isn't a table, return false
        if (!lua_istable(vm, -1))
        {
            throw LuaException("LuaManager::GetField - Cannot get field from unrecognised table.");
        }
        // Now we know that we have a table, try getting a needed field from it
        lua_getfield(vm, -1, field.c_str());
        // If that value is nil, return false
        if (lua_isnil(vm, -1))
        {
            throw LuaException("LuaManager::GetField - Field '" + field + "' does not exist in this table.");
        }
    }

    void LuaManager::GetField(const std::string& varName, const std::string& field)
    {
        // Pushes table to the top of the stack
        GetGlobal(varName);
        // Using table at the top of the stack, pushes field to the top of the stack
        GetField(field);
    }

    void LuaManager::SetGlobal(const std::string& varName)
    {
        // Gives the requested variable the value at the top of Lua's stack
        lua_setglobal(vm, varName.c_str());
    }

    void LuaManager::SetField(const std::string& field)
    {
        // Makes sure variable at index -2 is a table first
        if (!lua_istable(vm, -2))
        {
            throw LuaException("LuaManager::GetField - Cannot get field from unrecognised table.");
        }
        // Now sets the requested field of the table
        lua_setfield(vm, -2, field.c_str());
    }

    int LuaManager::GetIntFromVariable(const std::string& varName)
    {
        // Pushes needed variable onto the top of the stack
        GetGlobal(varName);
        // Makes sure the variable contains a number...
        if (lua_isnumber(vm, -1))
        {
            // ...if it does, return it...
            return luaL_checkinteger(vm, -1);
        }
        else
        {
            // ...if it doesn't, throw an exception...
            throw LuaException("LuaManager::GetIntFromVariable - '" + varName + "' is not a number!");
        }
        /* ...This procedure is done for every Get() method in the LuaManager, except for
         * strings, which just convert other types (such as numbers and booleans) into a
         * string before returning the value. */
    }

    std::string LuaManager::GetStringFromVariable(const std::string& varName)
    {
        GetGlobal(varName);
        return luaL_checkstring(vm, -1);
    }

    float LuaManager::GetFloatFromVariable(const std::string& varName)
    {
        GetGlobal(varName);
        if (lua_isnumber(vm, -1))
        {
            return luaL_checknumber(vm, -1);
        }
        else
        {
            throw LuaException("LuaManager::GetFloatFromVariable - '" + varName + "' is not a number!");
        }
    }

    double LuaManager::GetDoubleFromVariable(const std::string& varName)
    {
        GetGlobal(varName);
        if (lua_isnumber(vm, -1))
        {
            return luaL_checknumber(vm, -1);
        }
        else
        {
            throw LuaException("LuaManager::GetDoubleFromVariable - '" + varName + "' is not a number!");
        }
    }

    bool LuaManager::GetBoolFromVariable(const std::string& varName)
    {
        GetGlobal(varName);
        if (lua_isboolean(vm, -1))
        {
            return lua_toboolean(vm, -1);
        }
        else
        {
            throw LuaException("LuaManager::GetBoolFromVariable - '" + varName + "' is not a boolean!");
        }
    }

    int LuaManager::GetIntFromTable(const std::string& varName, const std::string& field)
    {
        /* Pushes table onto the top of the stack, then pushes the requested field
         * INSIDE that table onto the top of the stack and returns that value. */
        GetField(varName, field);
        if (lua_isnumber(vm, -1))
        {
            return luaL_checkinteger(vm, -1);
        }
        else
        {
            throw LuaException("LuaManager::GetIntFromTable - '" + varName + "." + field + "' is not a number!");
        }
    }

    std::string LuaManager::GetStringFromTable(const std::string& varName, const std::string& field)
    {
        GetField(varName, field);
        return luaL_checkstring(vm, -1);
    }

    float LuaManager::GetFloatFromTable(const std::string& varName, const std::string& field)
    {
        GetField(varName, field);
        if (lua_isnumber(vm, -1))
        {
            return luaL_checknumber(vm, -1);
        }
        else
        {
            throw LuaException("LuaManager::GetFloatFromTable - '" + varName + "." + field + "' is not a number!");
        }
    }

    double LuaManager::GetDoubleFromTable(const std::string& varName, const std::string& field)
    {
        GetField(varName, field);
        if (lua_isnumber(vm, -1))
        {
            return luaL_checknumber(vm, -1);
        }
        else
        {
            throw LuaException("LuaManager::GetDoubleFromTable - '" + varName + "." + field + "' is not a number!");
        }
    }

    bool LuaManager::GetBoolFromTable(const std::string& varName, const std::string& field)
    {
        GetField(varName, field);
        if (lua_isboolean(vm, -1))
        {
            return lua_toboolean(vm, -1);
        }
        else
        {
            throw LuaException("LuaManager::GetBoolFromTable - '" + varName + "." + field + "' is not a boolean!");
        }
    }

    void LuaManager::SetIntToVariable(const std::string& varName, int value)
    {
        // Pushes value to set at the top of the stack (-1) and calls SetGlobal()
        lua_pushnumber(vm, value);
        SetGlobal(varName);
    }

    void LuaManager::SetFloatToVariable(const std::string& varName, float value)
    {
        lua_pushnumber(vm, value);
        SetGlobal(varName);
    }

    void LuaManager::SetDoubleToVariable(const std::string& varName, double value)
    {
        lua_pushnumber(vm, value);
        SetGlobal(varName);
    }

    void LuaManager::SetBoolToVariable(const std::string& varName, bool value)
    {
        lua_pushboolean(vm, value);
        SetGlobal(varName);
    }

    void LuaManager::SetStringToVariable(const std::string& varName, const std::string& value)
    {
        lua_pushstring(vm, value.c_str());
        SetGlobal(varName);
    }

    void LuaManager::SetIntToTable(const std::string& varName, const std::string& field,
        int value)
    {
        // Pushes requested global to the top of the stack, followed by the value
        GetGlobal(varName); // Table will be at index -2
        lua_pushnumber(vm, value); // Value will be at index -1
        // Now sets the field (-1) in the table (-2)
        SetField(field);
    }

    void LuaManager::SetFloatToTable(const std::string& varName, const std::string& field,
        float value)
    {
        GetGlobal(varName);
        lua_pushnumber(vm, value);
        SetField(field);
    }

    void LuaManager::SetDoubleToTable(const std::string& varName, const std::string& field,
        double value)
    {
        GetGlobal(varName);
        lua_pushnumber(vm, value);
        SetField(field);
    }

    void LuaManager::SetBoolToTable(const std::string& varName, const std::string& field,
        bool value)
    {
        GetGlobal(varName);
        lua_pushboolean(vm, value);
        SetField(field);
    }

    void LuaManager::SetStringToTable(const std::string& varName, const std::string& field,
        const std::string& value)
    {
        GetGlobal(varName);
        lua_pushstring(vm, value.c_str());
        SetField(field);
    }

}

}
